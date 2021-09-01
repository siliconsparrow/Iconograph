// Pin assignments and related definitions for the Iconograph hardware.
// by Adam Pierce <adam@siliconsparrow.com>
// created 15-May-2021

#ifndef _BOARD_H_
#define _BOARD_H_

// Definitions for the chip itself.
#include "nrf.h"
//#include "nrf52.h"
//#include "nrf52_bitfields.h"

// Pinout
enum Pinout {
	// Symbol      Port  Description
	// ---------   ----  ----------------------------------------------
	pinPwmMotor1 = 4,    // P0.04 - PWM output for servo motor 1
	pinPwmMotor2 = 5,    // P0.05 - PWM output for servo motor 2
	pinPwmMotor3 = 6,    // P0.06 - PWM output for servo motor 3
	pinPwmMotor4 = 7,    // P0.07 - PWM output for servo motor 4
};

#endif // _BOARD_H_
