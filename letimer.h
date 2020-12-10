/*****************************************
 * File: letimer.h
 * Description: This file implements a periodic Letimer,
 * 		used to trigger capacitive sensing operations
 *
 */



#include <stdint.h>
#include <stdbool.h>


// Desired frequency in Hz
#define OUT_FREQ 10


void LETIMERinit(void);
void LETIMERstart(void);
