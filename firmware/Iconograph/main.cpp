// Iconograph firmware for the nRF52932-QFAA
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

// I'm using the Nordic Softdevice system s132 v7.0.1 to provide the Bluetooth stack. It also
// provides other chip functions. The following peripherals MUST be accessed via
// the softdevice API (see the softdevice specification document):
//
// * CLOCK
// * POWER
// * BPROT
// * RADIO
// * TIMER0
// * RTC0
// * TEMP
// * RNG
// * ECB
// * CCM
// * AAR
// * EGU1/SWI1
// * EGU2/SWI2
// * EGU4/SWI4
// * EGU5/SWI5
// * NVMC
// * MWU
// * FICR
// * UICR
// * NVIC

// Interrupt priorities
//
// -3 - Reset
// -2 - NMI
// -1 - Hardfault
//  0 - Softdevice timing critical (BLE radio)
//  1 - Softdevice memory management
//  2 -
//  3 -
//  4 - Softdevice API
//  5 - Reserved for Softdevice future use
//  6 -
//  7 -

#include "Iconograph.h"
#include "nrf_sdh.h"

extern "C" void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
	while(1)
		;
}

// Set up the softdevice and bluetooth stack.
// This must be done as soon as possible after boot.
void chipInit()
{
	nrf_sdh_enable_request();

#ifdef ENABLE_BLE
	// Configure the BLE stack using the default settings.
	// Fetch the start address of the application RAM.
	uint32_t ram_start = 0;
	err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
	APP_ERROR_CHECK(err_code);

	// Enable BLE stack.
	err_code = nrf_sdh_ble_enable(&ram_start);
	APP_ERROR_CHECK(err_code);

	// Register a handler for BLE events.
	NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, evtHandler, NULL);
#endif // ENABLE_BLE

	// Register handler for SOC events such as flash memory writes.
	//NRF_SDH_SOC_OBSERVER(m_soc_observer, APP_SOC_OBSERVER_PRIO, soc_evt_handler, NULL);
}

int main()
{
	// Set up CPU and basic chip stuff.
	chipInit();

	// Create the application.
	Iconograph iconograph;

	// Main loop. Just sleep. Everything from
	// here on happens with interrupts.
	while(1) {
#ifdef OLD
		sleep();
#endif // OLD
	}
}


#if 0

// I'm using the Nordic Softdevice system s132 v7.0.1 to provide the Bluetooth stack. It also
// provides other chip functions. The following peripherals MUST be accessed via
// the softdevice API (see the softdevice specification document):
//
// * CLOCK
// * POWER
// * BPROT
// * RADIO
// * TIMER0
// * RTC0
// * TEMP
// * RNG
// * ECB
// * CCM
// * AAR
// * EGU1/SWI1
// * EGU2/SWI2
// * EGU4/SWI4
// * EGU5/SWI5
// * NVMC
// * MWU
// * FICR
// * UICR
// * NVIC

/*
Interrupt priorities

-3 - Reset
-2 - NMI
-1 - Hardfault
 0 - Softdevice timing critical (BLE radio)
 1 - Softdevice memory management
 2 - Watchdog, Realtime clock
 3 - UART
 4 - Softdevice API
 5 - Reserved for Softdevice future use
 6 - I2C, GeneralTimer
 7 -

SPI uses DMA and does not use an interrupt.
*/

#include "NRF52Common.h"
#include "BleScanner.h"
#include "BootloaderData.h"
#include "Watchdog.h"
#include "GeneralTimer.h"
#include "LP4242.h"
#include <stdint.h>

uint8_t g_resetReason; // Copy of the CPU-supplied reset reason.


// ********************
// ** START HERE
// **

int main()
{
    // Find out the reason for the last restart (power loss, crash etc.)
    g_resetReason = LevauxDriver::NRF52Common::getResetReason();

    // Set up the Watchdog timer.
    LevauxDriver::Watchdog watchdog;

    // SoftDevice initialization. Also sets up CPU clocks.
    Levaux::BleScanner::softdeviceInit();
    Levaux::BleScanner::enableHfClock(); // Set the crystal to run all the time. Uses more power but is more stable.

    // Make sure the general timer has been initialized.
    LevauxDriver::GeneralTimer::instance();

    // Create the main class that controls everything.
    Levaux::LP4242 lp4242;

#ifdef PROFILER_ENABLE
    // Start up the profiler.
    profileInit();
#endif // PROFILER_ENABLE

    while(1)
    {
        // Keep the watchdog happy.
        watchdog.reset();

        // This is where all the work happens.
        lp4242.tick();
    }
}

#endif
