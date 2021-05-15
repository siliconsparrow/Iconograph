// Iconograph main class
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#ifndef ICONOGRAPH_H_
#define ICONOGRAPH_H_

#include "BLE.h"
#include "Motor.h"
#include "Timer.h"

class Iconograph
	: public Timer::Delegate
{
public:
	Iconograph();
	virtual ~Iconograph() { }

	virtual void evtTimer(unsigned param);

private:
	enum {
		kNumMotors = 3,
	};

	BLE   _ble;
	Motor _motor;

	unsigned _motorNum; // TEST
	unsigned _motorPos; // TEST
};


#endif // ICONOGRAPH_H_
