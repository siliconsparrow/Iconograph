// Iconograph firmware for the nRF52932-QFAA
// by Adam Pierce <adam@siliconsparrow.com>
// created 9-May-2021

// TODO: Watchdog
// TODO: Robot arm control
// TODO: Packet structure and vector drawing

#include "nordic_common.h"
#include "nrf.h"
#include "nrf_sdh.h"
#include "app_timer.h"
#include "app_util_platform.h"
#include "nrf_pwr_mgmt.h"
#include "Iconograph.h"

// Called when there is a Softdevice error. Freeze and wait for the
// watchdog to reset us.
extern "C" void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
	while(1)
		;
}

int main()
{
    ret_code_t err_code;

    // Set up timers for Softdevice.
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    // Set up power management (so we can idle the CPU when not in use).
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

    // Set up Softdevice.
    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Create our main application.
	Iconograph iconograph;

	// The rest will be done with interrupts.

    while(1) {
    	// Sleep or idle or something while we have nothing to do.
    	nrf_pwr_mgmt_run();
    }
}
