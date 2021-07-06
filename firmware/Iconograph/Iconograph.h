// Iconograph main class
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#ifndef ICONOGRAPH_H_
#define ICONOGRAPH_H_

#include "BLEUart.h"
#include "Motor.h"
#include "Timer.h"

class Iconograph
	: public BLEUart::Delegate
	, public Timer::Delegate
{
public:
	Iconograph();
	virtual ~Iconograph() { }

protected:
	virtual void rxData(const uint8_t *data, unsigned size);
	virtual void evtTimer(unsigned param);

private:
	enum {
		kNumMotors = 3,
	};

	Motor  _motor;
	BLEUart _uart;

	unsigned _motorNum; // TEST
	unsigned _motorPos; // TEST
};


#endif // ICONOGRAPH_H_
