// Iconograph main class
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

#include "Iconograph.h"
#include "BLE.h"

#if OLD

Iconograph::Iconograph()
	: _motorNum(0)
	, _motorPos(0)
{
	_ble = BLE::instance();

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
#endif // OLD


Iconograph::Iconograph()
{
	_uart.setDelegate(this);
}

// Called when data is received from the Android app.
void Iconograph::rxData(const uint8_t *data, unsigned size)
{
	static char code = 'A';
	unsigned maxPayload = _uart.getMaxPayload();

	char msg[maxPayload];
	strcpy(msg, "ECHO:_:");

	msg[5] = code++;

	unsigned len = size;
	if(len > maxPayload - 7)
		len = maxPayload - 7;

	memcpy(&msg[7], data, len);
	_uart.send((const uint8_t *)msg, len + 7);
}
