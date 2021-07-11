// Iconograph main class
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#include "Iconograph.h"
#include <math.h>

#define PI 3.1415726535

enum {
	kRodLength1  = 90,   // Length of first rod in mm
	kRodLength2  = 80,   // Length of second rod in mm
	kHorizRange  = 130,  // Maximum horizontal range of positions (mm)
	kHorizOrigin = 0,    // Minimum horizontal position (mm relative to shoulder joint)
	kVertRange   = 80,   // Maximum vertical range of positions (mm)
	kVertOrigin  = 30,   // Minimum vertical position (mm relative to shoulder joint)
	kMotor0min   = 500,  // Minimum position of shoulder motor
	kMotor0Max   = 1500, // Maximum position of shoulder motor
	kMotor1min   = 0,    // Minimum position of elbow motor
	kMotor1max   = 2000, // Maximum position of elbow motor
};

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
	uint8_t cmd = data[0];
	data++;
	size--;

	switch(cmd)
	{
	case 2: // Direct motor control
		cmdMotorPosition(data, size);
		break;

	case 4: // X-Y positioning
		cmdXyPosition(data, size);
		break;

	default:
		break;
	}
}

void Iconograph::cmdMotorPosition(const uint8_t *data, unsigned size)
{
	unsigned motorNum = data[0] - 1;
	if(motorNum >= Motor::kMotorChannels) {
		return;
	}

	_motor.setPosition(motorNum, ((100 - (unsigned)data[1]) * Motor::getMaxPosition()) / 100);
}

void Iconograph::cmdXyPosition(const uint8_t *data, unsigned size)
{
	struct __attribute__((packed)) CmdXy
	{
		uint16_t x;
		uint16_t y;
	};

	if(size < sizeof(CmdXy)) {
		return;
	}

	const CmdXy *xy = (const CmdXy *)data;
	moveTo(xy->x, xy->y);
}

// Calculate motor angles to position the end effector at the given x,y coordinates.
// Coordinates range from 0 to 65535 and are scaled to the arm's reach.
void Iconograph::moveTo(uint16_t posX, uint16_t posY)
{
	// Scale the coordinates to our printable area.
	double x = (((double)posX * kHorizRange) / 65535) + kHorizOrigin;
	double y = (((double)posY * kVertRange) / 65535) + kVertOrigin;

	// Calculate the distance from the pivot to the coordinates.
	double d = sqrt((x*x)+(y*y));

	// Calculate the elbow angle.
	double thElbow = calcOppositeAngle(d, kRodLength1, kRodLength2);

	// Calculate the shoulder angle.
	double thMid = calcOppositeAngle(kRodLength1, kRodLength2, d);

	// Calculate the angle between the shoulder and the horizontal axis.
	double thShoulder = PI - atan(y/(-x)) - thMid;

	// Convert angles into motor positions (0-2000)
	double motShoulder = (2000 * thShoulder) / PI;
	double motElbow    = (2000 * (thElbow - PI)) / PI;
	if(motShoulder < kMotor0min || motShoulder > kMotor0Max || motElbow < kMotor1min || motElbow > kMotor1max) {
		return;
	}
	_motor.setPosition(0, motShoulder);
	_motor.setPosition(1, motElbow);
}

// Use the law of cosines to calculate one angle of a triangle when
// given the length of all three sides.
double Iconograph::calcOppositeAngle(double a, double b, double c)
{
	return acos(((a*a)+(b*b)-(c*c)) / (2 * a * b));
}
