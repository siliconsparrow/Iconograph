// Servo motor controller for the nRF52
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

class Motor
{
public:
	enum {
		kMotorChannels = 4,
	};

	Motor();

	static unsigned getMaxPosition();

	void setPosition(unsigned motorNum, unsigned pos);

private:
	uint16_t _seq[kMotorChannels];
	unsigned _currentSeq;
};

#endif // MOTOR_H_
