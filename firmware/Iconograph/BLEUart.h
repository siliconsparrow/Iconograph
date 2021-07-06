/*
 * BLEUart.h
 *
 *  Created on: 6 Jul 2021
 *      Author: adam
 */

#ifndef BLEUART_H_
#define BLEUART_H_

#include <stdint.h>
#include "nrf_ble_gatt.h"

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
		virtual void connectionStateChanged(bool connected) { }
	};

	BLEUart();

	void setDelegate(Delegate *delegate) { _delegate = delegate; }

	unsigned getMaxPayload() const;

	void send(const uint8_t *data, uint16_t length);

private:
	Delegate *_delegate;
	uint16_t  _bleNusMaxDataLen;
	uint16_t  _connHandle;

	void bleStackInit();
	void connParamsInit();
	void gapParamsInit();
	void gattInit();
	void servicesInit();
	void advertisingInit();
	void advertisingStart();

public: // Handlers. Do not call these directly!
	void rxDataHandler(const uint8_t *data, unsigned length);
	void evtConnectionFailed();
	void evtGatt(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt);
	void evtBle(ble_evt_t const *p_ble_evt);
};

#endif // BLEUART_H_
