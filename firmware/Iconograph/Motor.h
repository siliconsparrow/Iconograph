// Servo motor controller for the nRF52
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#ifndef MOTOR_H_
#define MOTOR_H_

#include "MotorPwm.h"
#include "MotorTimer.h"

class Motor
    : public MotorTimer::Delegate
{
public:
	enum {
		kMaxPosition = MotorPwm::kMaxValue,
		kMaxMotors   = 4,
	};

	Motor(unsigned pwmNum, unsigned pwmChannel);
	virtual ~Motor() { }

	void moveTo(unsigned pos);

protected:
	virtual void evtMotorTimer();

private:
	MotorPwm *_pwm;
	unsigned  _pwmChannel;
	int       _pos;
	int       _speed;
	int       _target;
};

#endif // MOTOR_H_
