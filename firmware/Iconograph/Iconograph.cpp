// Iconograph main class
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#include "Iconograph.h"

Iconograph::Iconograph()
	: _motorNum(0)
	, _motorPos(0)
{
	Timer::instance()->setPeriodic(250, this);
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
