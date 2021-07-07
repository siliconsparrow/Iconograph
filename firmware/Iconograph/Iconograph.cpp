// Iconograph main class
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#include "Iconograph.h"

Iconograph::Iconograph()
	: _motorNum(0)
	, _motorPos(0)
{
	_uart.setDelegate(this);

	//Timer::instance()->setPeriodic(250, this);

	_motor.setPosition(0, Motor::getMaxPosition() / 2);
	_motor.setPosition(1, Motor::getMaxPosition() / 2);
}

void Iconograph::evtTimer(unsigned param)
{
	_motor.setPosition(_motorNum++, _motorPos);

	if(_motorNum >= kNumMotors) {
		_motorNum = 0;
		_motorPos += (Motor::getMaxPosition() / 4);
		if(_motorPos > Motor::getMaxPosition()) {
			_motorPos = 0;
		}
	}
}

// Called when data is received from the Android app.
void Iconograph::rxData(const uint8_t *data, unsigned size)
{
	if(data[0] == 2) {
		switch(data[1])
		{
		case 1:
			_motor.setPosition(0, ((100 - (unsigned)data[2]) * Motor::getMaxPosition()) / 100);
			break;

		case 2:
			_motor.setPosition(1, ((100 - (unsigned)data[2]) * Motor::getMaxPosition()) / 100);
			break;
		}
	}
}
