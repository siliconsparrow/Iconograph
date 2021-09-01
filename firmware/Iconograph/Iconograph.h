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
		kMotorShoulder,
		kMotorElbow,
		kMotorPen,

		kNumMotors = 3,
	};

	Motor   *_motor[kNumMotors];
	BLEUart _uart;

	unsigned _motorNum; // TEST
	unsigned _motorPos; // TEST
	float    _theta;

	void cmdMotorPosition(const uint8_t *data, unsigned size);
	void cmdXyPosition(const uint8_t *data, unsigned size);
	void cmdPenUpDown(uint8_t up);
	void cmdResetPosition();
	void moveTo(uint16_t posX, uint16_t posY);

	static float calcOppositeAngle(float a, float b, float c);
};

#endif // ICONOGRAPH_H_
