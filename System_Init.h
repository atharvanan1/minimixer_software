/*
 * System_Init.h
 *
 *  Created on: Nov 21, 2020
 *      Author: Jiabin Lin
 */

#ifndef SRC_SYSTEM_INIT_H_
#define SRC_SYSTEM_INIT_H_

#define PBSTAT_PORT gpioPortC
#define PBSTAT_PIN  7

#define AO_PORT		gpioPortC
#define AO_PIN		11

#define POWERON 	1
#define POWEROFF 	0


void POWER_INIT();
void POWER_DEINIT();
void PBstatus();
void PB_Evt(uint32_t iflag);

void Battery_status();
void AOstatus();
void Battery_status();

#endif /* SRC_SYSTEM_INIT_H_ */
