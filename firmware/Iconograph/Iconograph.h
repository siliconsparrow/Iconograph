// Iconograph main class
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#ifndef ICONOGRAPH_H_
#define ICONOGRAPH_H_

#include "BLEUart.h"
//#include "Motor.h"
//#include "Timer.h"

#ifdef OLD

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

	Motor  _motor;

	unsigned _motorNum; // TEST
	unsigned _motorPos; // TEST
};
#endif // OLD

class Iconograph
	: public BLEUart::Delegate
{
public:
	Iconograph();
	virtual ~Iconograph() { }

protected:
	virtual void rxData(const uint8_t *data, unsigned size);

private:
	BLEUart _uart;
};


#endif // ICONOGRAPH_H_
