/*
 * @file midi.c
 *
 *  Created on: 2019.05.31.
 *      Author: baadamff
 */
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_core.h"
#include "em_common.h"
#include "em_cryotimer.h"
#include "gpiointerrupt.h"

#include "hal-config.h"
#include "native_gecko.h"
#include "gatt_db.h"

#include "midi.h"
#include "app.h"

// Notes to MIDI code
#define    C__4 60
#define    Ci_4 61
#define    D__4 62
#define    Di_4 63
#define    E__4 64
#define    F__4 65
#define    Fi_4 66
#define    G__4 67
#define    Gi_4 68
#define    A__4 69
#define    Ai_4 70
#define    B__4 71
#define    C__5 72
#define    Ci_5 73
#define    D__5 74
#define    Di_5 75
#define    E__5 76
#define    F__5 77
#define    Fi_5 78
#define    G__5 79
#define    Gi_5 80
#define    A__5 81
#define    Ai_5 82
#define    B__5 83

PACKSTRUCT( typedef struct
{
  uint8  header;
  uint8  timestamp;
  uint8  status;
  uint8  note;
  uint8  velocity;

})midi_event_packet_t;

typedef union {
  midi_event_packet_t  midi_event;
  uint8 payload[5];
}midi_data_t;

static uint8_t connectionId;

#define MELODY_SIZE 18
static uint8_t melody[MELODY_SIZE] = {A__4, A__4, A__4, F__4, C__5, A__4, F__4, C__5, A__4,
                    E__5, E__5, E__5, F__5, C__5, Gi_4, F__4, C__5, A__4};
static uint8_t playhead = 0;

void midi_note_on(uint8 note, uint8 velocity);
void midi_note_off(uint8 note, uint8 velocity);
void PB0_pressed(uint8_t pin);
void PB1_pressed(uint8_t pin);

void midi_init_buttons(void)
{
  // Init GPIO ISR dispatcher
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIOINT_Init();

  // Setup PB0
  // GPIO init for button pins
//  GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInput, 0);
//
//  // Register GPIO ISR
//  GPIOINT_CallbackRegister(BSP_BUTTON0_PIN, PB0_pressed);
//
//  // Trigger for both edges
//  GPIO_IntConfig(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, true, true, true);
//
//  // Setup PB1
//  // GPIO init for button pins
//  GPIO_PinModeSet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, gpioModeInput, 0);
//
//  // Register GPIO ISR
//  GPIOINT_CallbackRegister(BSP_BUTTON1_PIN, PB1_pressed);
//
//  // Trigger for both edge
//  GPIO_IntConfig(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, true, true, true);
}

void midi_init_ble_connection(void)
{
  gecko_cmd_sm_configure(0x0F, sm_io_capability_noinputnooutput);
  gecko_cmd_sm_set_bondable_mode(1);
}

void midi_ble_connected(uint8_t connection)
{
  // Store the connection ID
  connectionId = connection;
  // Connection parameter constrains for BLE MIDI
  gecko_cmd_le_connection_set_timing_parameters(connection, 0x12, 0x12, 0, 0x64, 0, 0xFFFF);
}

void midi_ble_disconnected(void)
{
  connectionId = 0xFF;
  midi_note_off(melody[playhead], 100);
  playhead = 0;
}

void PB0_pressed(uint8_t pin)
{
////  if (1 == GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN))
//  {
//    midi_note_off(melody[playhead], 100);
//
//    // Next note in melody
//    playhead++;
//
//    // If the melody is over reset the playhead
//    if (playhead >= MELODY_SIZE)
//    {
//      playhead = 0;
//    }
//  }
//  else
//  {
//    midi_note_on(melody[playhead], 100);
//  }
}

void PB1_pressed(uint8_t pin)
{
  // Start the melody from beginning
  midi_note_off(melody[playhead], 100);
  playhead = 0;
}

void midi_note_on(uint8 note, uint8 velocity)
{
  static midi_data_t note_on;

  uint32 temp = CRYOTIMER_CounterGet();                    // Get the timestamp from the CRYOTIMER
  temp = temp & 0x00001fff;                                // Mask it - only the lower 13 bit needed

  note_on.midi_event.header     = 0x80 | (temp >> 7);     // Header byte = 0b10xxxxxx where xxxxxxx is top 6 bits of timestamp
  note_on.midi_event.timestamp  = 0x80 | (temp & 0x003f); // Timestamp byte = 0b1xxxxxxx where xxxxxxx is lower 7 bits of timestamp
  note_on.midi_event.status     = 0x90;                   // Status byte = 0b1sssnnnn where sss is message type and nnnn is channel
  note_on.midi_event.note       = note;                   // Setting the note parameter
  note_on.midi_event.velocity   = velocity;               // Setting the velocity parameter

  // Sending the assembled midi message
  printLog("Note On\r\n");
  gecko_cmd_gatt_server_send_characteristic_notification(connectionId, gattdb_xgatt_midi, 5, (uint8 const *) &note_on);
}

void midi_note_off(uint8 note, uint8 velocity)
{
  static midi_data_t note_off;

  uint32 temp = CRYOTIMER_CounterGet();                      // 1ms timestamp
  temp = temp & 0x00001fff;                                  // Only the lower 13 bit needed

  note_off.midi_event.header    = 0x80 | (temp >> 7);       // Header byte = 0b10xxxxxx where xxxxxxx is top 6 bits of timestamp
  note_off.midi_event.timestamp = 0x80 | (temp & 0x003f);   // Timestamp byte = 0b1xxxxxxx where xxxxxxx is lower 7 bits of timestamp
  note_off.midi_event.status    = 0x80;                     // Status byte = 0b1sssnnnn where sss is message type and nnnn is channel
  note_off.midi_event.note      = note;                     // Setting the note parameter
  note_off.midi_event.velocity  = velocity;                 // Setting the velocity parameter

//  printLog("Note Off\r\n");
  gecko_cmd_gatt_server_send_characteristic_notification(connectionId, gattdb_xgatt_midi, 5, (uint8 const*)&note_off);
}

void midi_init_timer(void)
{
  CRYOTIMER_Init_TypeDef cryoInit = CRYOTIMER_INIT_DEFAULT;

  // General settings
  cryoInit.enable    = 1;
  cryoInit.debugRun  = 0;
  cryoInit.em4Wakeup = 0;

  // Clocking settings
  // With a frequency of 1000Hz on ULFRCO, this will result in a 1.00 ms timeout
  cryoInit.osc    = cryotimerOscULFRCO;
  cryoInit.presc  = cryotimerPresc_1;
  cryoInit.period = cryotimerPeriod_1;
  CRYOTIMER_Init(&cryoInit);
}

