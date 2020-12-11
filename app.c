/***************************************************************************//**
 * @file app.c
 * @brief Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

#include "midi.h"
#include "app.h"
#include "events.h"
#include "capsense.h"
#include "letimer.h"

/* Print boot message */
static void bootMessage(struct gecko_msg_system_boot_evt_t *bootevt);

/* Flag for indicating DFU Reset must be performed */
static uint8_t boot_to_dfu = 0;

static uint8_t capsense_channel = 0;

/* Main application */
void appMain(gecko_configuration_t *pconfig)
{
#if DISABLE_SLEEP > 0
  pconfig->sleep.flags = 0;
#endif

  /* Initialize debug prints. Note: debug prints are off by default. See DEBUG_LEVEL in app.h */
  initLog();

  /* Initialize stack */
  gecko_init(pconfig);

  while (1) {
    /* Event pointer for handling events */
    struct gecko_cmd_packet* evt;

    /* if there are no events pending then the next call to gecko_wait_event() may cause
     * device go to deep sleep. Make sure that debug prints are flushed before going to sleep */
    if (!gecko_event_pending()) {
      flushLog();
    }

    /* Check for stack event. This is a blocking event listener. If you want non-blocking please see UG136. */
    evt = gecko_wait_event();

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {
      /* This boot event is generated when the system boots up after reset.
       * Do not call any stack commands before receiving the boot event.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

    	midi_init_ble_connection();
    	LETIMERstart();
        bootMessage(&(evt->data.evt_system_boot));
        printLog("boot event - starting advertising\r\n");

        /* Set advertising parameters. 100ms advertisement interval.
         * The first parameter is advertising set handle
         * The next two parameters are minimum and maximum advertising interval, both in
         * units of (milliseconds * 1.6).
         * The last two parameters are duration and maxevents left as default. */
        gecko_cmd_le_gap_set_advertise_timing(0, 160, 160, 0, 0);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
        break;

      case gecko_evt_le_connection_opened_id:

        printLog("connection opened\r\n");
        midi_ble_connected(evt->data.evt_le_connection_opened.connection);
        break;

      case gecko_evt_le_connection_closed_id:

        printLog("connection closed, reason: 0x%2.2x\r\n", evt->data.evt_le_connection_closed.reason);
        midi_ble_disconnected();

        /* Check if need to boot to OTA DFU mode */
        if (boot_to_dfu) {
          /* Enter to OTA DFU mode */
          gecko_cmd_system_reset(2);
        } else {
          /* Restart advertising after client has disconnected */
          gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
        }
        break;

      /* Events related to OTA upgrading
         ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:
        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection,
            gattdb_ota_control,
            bg_err_success);

          /* Close connection to enter to DFU OTA mode */
          gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
        }
        break;

      /* Add additional event handlers as your application requires */

      case gecko_evt_gatt_server_attribute_value_id:
    	  /* This event will handle Playing the notes and calling Codec APIs */
    	  printLog("Note play\r\n");
    	  if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_xgatt_midi) {
    	      // Write user supplied value to LEDs.
    	      printLog("Command - %02x; ", evt->data.evt_gatt_server_attribute_value.value.data[2]);
    	      printLog("Note - %d; ", evt->data.evt_gatt_server_attribute_value.value.data[3]);
    	      printLog("Velocity - %d\r\n", evt->data.evt_gatt_server_attribute_value.value.data[4]);
    	      gecko_cmd_gatt_server_send_user_write_response(evt->data.evt_gatt_server_user_write_request.connection, gattdb_xgatt_midi, bg_err_success);
    	  }
    	  break;

     // Capacitive Sensing Event
      case gecko_evt_system_external_signal_id:
    	  switch(evt->data.evt_system_external_signal.extsignals) {
    	  	  case CAP_MEASURE_START:
    	  		  capsense_channel = 0;
    	  		  CAPSENSE_Start_Measurement(capsense_channel);
    	  		  break;

    	  	  case CAP_MEASURE_END:
    	  		  capsense_channel++;

    	  		  if(capsense_channel == ACMP_CHANNELS){

    	  			  printLog("Cap Measurement Finished");

					  if(CAPSENSE_getPressed(0)){
						GPIO_PinOutSet(gpioPortD, 14);
					  }
					  else{
						GPIO_PinOutClear(gpioPortD, 14);
					  }

					  if(CAPSENSE_getPressed(1)){
						GPIO_PinOutSet(gpioPortD, 15);
					  }
					  else{
						GPIO_PinOutClear(gpioPortD, 15);
					  }
					  LETIMERstart();
    	  		  }

    	  		  else{
    	  			  CAPSENSE_Start_Measurement(capsense_channel);
    	  		  }

    	  		  break;

    	  	  case ENC_0:

    	  		  break;

    	  	  default:
    	  		  printLog("UnknownExternalSignal");
    	  		  break;
    	  }

    	  break;

      default:
        break;
    }
  }
}

/* Print stack version and local Bluetooth address as boot message */
static void bootMessage(struct gecko_msg_system_boot_evt_t *bootevt)
{
#if DEBUG_LEVEL
  bd_addr local_addr;
  int i;

  printLog("stack version: %u.%u.%u\r\n", bootevt->major, bootevt->minor, bootevt->patch);
  local_addr = gecko_cmd_system_get_bt_address()->address;

  printLog("local BT device address: ");
  for (i = 0; i < 5; i++) {
    printLog("%2.2x:", local_addr.addr[5 - i]);
  }
  printLog("%2.2x\r\n", local_addr.addr[0]);
#endif
}
