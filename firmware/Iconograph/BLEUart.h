/*
 * BLEUart.h
 *
 *  Created on: 6 Jul 2021
 *      Author: adam
 */

#ifndef BLEUART_H_
#define BLEUART_H_

#include <stdint.h>
#include "ble_gatt.h"

// Based on the Nordic UART example, this class uses Softdevice s112 to
// provide a serial communication link over Bluetooth BLE.
class BLEUart
{
public:
	class Delegate
	{
	public:
		virtual ~Delegate() { }

		virtual void rxData(const uint8_t *data, unsigned size) = 0;
	};

	BLEUart();

	void setDelegate(Delegate *delegate) { _delegate = delegate; }

	unsigned getMaxPayload() const;

	void send(const uint8_t *data, unsigned size);

private:
	Delegate *_delegate;

	void bleStackInit();
	void advertisingStart();

public: // Handlers. Do not call these directly!
	void rxDataHandler(const uint8_t *data, unsigned length);
};

#endif // BLEUART_H_
