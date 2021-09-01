// Servo motor controller for the nRF52
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

// The MG90S servomotor is driven by a PWM signal. Pulse frequency is
// 50Hz (20ms). The pulses range from 400us to 2400us to control the
// position of the shaft.

// I'll be using the nRF52's built-in PWM peripheral which can control
// up to four channels simultaneously.

#include "Motor.h"
#include "board.h"

Motor::Motor(unsigned pwmNum, unsigned pwmChannel)
    : _pwm(MotorPwm::instance(pwmNum))
	, _pwmChannel(pwmChannel)
	, _pos(kMaxPosition / 2)
	, _speed(0)
	, _target(kMaxPosition / 2)
{
	MotorTimer::instance()->subscribe(this);
}


/*
 * This needs big revising.
 * I think the acceleration should start with an exponent until max speed is reached.
 * when the distance is less than a fixed amount from the end of travel, deceleration
 * is started with an exponent. If there is another queued vector and it is in the
 * same direction, deceleration is not neccessary.
 *
 * If the vector is an angle, the speed might need to be different for the X and Y motors.
 *
 */

void Motor::evtMotorTimer()
{
	enum {
		kDecelDistance = 100,
		kAccelStep     = 2,
	};

	int dx = _target - _pos; // How far until target position?
	int absdx = dx < 0 ? -dx : dx;
	if(absdx < kDecelDistance) {
		_speed = (dx * 9) / 10;
	} else if(dx < 0) {
		_speed -= kAccelStep;
	} else {
		_speed += kAccelStep;
	}

	_pos += _speed;

	_pwm->set(_pwmChannel, _pos);
}

void Motor::moveTo(unsigned pos)
{
	_target = pos;
}

#ifdef OLD
const uint32_t gMotorPinout[] = { pinPwmMotor1, pinPwmMotor2, pinPwmMotor3, pinPwmMotor4 };

// Which PWM peripheral to use. You can override this is board.h if required.
#ifndef MOTOR_DEVICE
	#define MOTOR_DEVICE NRF_PWM0
#endif


// Timing parameters for the MG90S servomotor.
enum {
	kMotorPwmPrescaler = PWM_PRESCALER_PRESCALER_DIV_16, // 1MHz base frequency. Resolution = 1us.
	kMotorPwmPeriod    = 20000,                          // 20000 counts for the period (20ms).
	kMotorPwmPolarity  = (1 << 15),                      // Idle low, pulse high.
	kMotorPulseMax     = 2000,                           // Motor full range is 2000 steps.
	kMotorMinValue     = 400,                            // Minimum pulse width (microseconds).
};

Motor *g_motorInstance = 0;

Motor::Motor()
	: _currentSeq(0)
{
	// Pin assignment.
	for(int chan = 0; chan < kMotorChannels; chan++) {
		MOTOR_DEVICE->PSEL.OUT[chan] = (gMotorPinout[chan] << PWM_PSEL_OUT_PIN_Pos)
				                  | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);
	}

	g_motorInstance = this;

	// Set up the PWM peripheral.
	MOTOR_DEVICE->ENABLE     = (PWM_ENABLE_ENABLE_Enabled << PWM_ENABLE_ENABLE_Pos);
	MOTOR_DEVICE->MODE       = (PWM_MODE_UPDOWN_Up << PWM_MODE_UPDOWN_Pos);
	MOTOR_DEVICE->PRESCALER  = (kMotorPwmPrescaler << PWM_PRESCALER_PRESCALER_Pos);
	MOTOR_DEVICE->COUNTERTOP = (kMotorPwmPeriod << PWM_COUNTERTOP_COUNTERTOP_Pos);
	MOTOR_DEVICE->LOOP       = (PWM_LOOP_CNT_Disabled << PWM_LOOP_CNT_Pos);
	MOTOR_DEVICE->DECODER    = (PWM_DECODER_LOAD_Individual << PWM_DECODER_LOAD_Pos)
			                 | (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos);

	// Set up sequence.
	for(int chan = 0; chan < kMotorChannels; chan++) {
		_seq[chan] = kMotorPwmPolarity | kMotorMinValue;
	}
	for(int seq = 0; seq < 2; seq++) {
		MOTOR_DEVICE->SEQ[seq].PTR = ((uint32_t)(_seq) << PWM_SEQ_PTR_PTR_Pos);
		MOTOR_DEVICE->SEQ[seq].CNT = ((sizeof(_seq) / sizeof(uint16_t)) << PWM_SEQ_CNT_CNT_Pos);
		MOTOR_DEVICE->SEQ[seq].REFRESH = 0;
		MOTOR_DEVICE->SEQ[seq].ENDDELAY = 0;
	}

	// Set up a timer for smooth animation.
	initTimer();

	// Start.
	MOTOR_DEVICE->TASKS_SEQSTART[_currentSeq] = 1;
}

unsigned Motor::getMaxPosition()
{
	return kMotorPulseMax;
}

// Change the duty cycle of the PWM to control the servo motor.
void Motor::setPosition(unsigned motorNum, unsigned pos)
{
	_seq[motorNum] = kMotorPwmPolarity | (kMotorMinValue + pos);

	_currentSeq = _currentSeq != 0 ? 0 : 1;
	MOTOR_DEVICE->TASKS_SEQSTART[_currentSeq] = 1;
}

void Motor::animateTo(unsigned motorNum, unsigned pos)
{
	m_motor[motorNum].
}

#endif // OLD
