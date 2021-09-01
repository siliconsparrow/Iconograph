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

enum CrashMethod {

    // CPU Exceptions.
    CRASH_NMI = 1,
    CRASH_HardFault = 2,
    CRASH_MemoryManagement = 3,
    CRASH_BusFault = 4,
    CRASH_UsageFault = 5,
    CRASH_SVC = 10,
    CRASH_DebugMon = 11,
    CRASH_PendSV = 13,

    // Unhandled interrupts.
    CRASH_POWER_CLOCK_IRQ = 16,
    CRASH_RADIO_IRQ = 17,
    CRASH_UART0_IRQ = 18,
    CRASH_SPI0_IRQ = 19,
    CRASH_SPI1_IRQ = 20,
    CRASH_NFCT_IRQ = 21,
    CRASH_GPIOTE_IRQ = 22,
    CRASH_SAADC_IRQ = 23,
    CRASH_TIMER0_IRQ = 24,
    CRASH_TIMER1_IRQ = 25,
    CRASH_TIMER2_IRQ = 26,
    CRASH_RTC0_IRQ = 27,
    CRASH_TEMP_IRQ = 28,
    CRASH_RNG_IRQ = 29,
    CRASH_ECB_IRQ = 30,
    CRASH_CCM_AAR_IRQ = 31,
    CRASH_WDT_IRQ = 32,
    CRASH_RTC1_IRQ = 33,
    CRASH_QDEC_IRQ = 34,
    CRASH_COMP_LPCOMP_IRQ = 35,
    CRASH_SWI0_EGU0_IRQ = 36,
    CRASH_SWI1_EGU1_IRQ = 37,
    CRASH_SWI2_EGU2_IRQ = 38,
    CRASH_SWI3_EGU3_IRQ = 39,
    CRASH_SWI4_EGU4_IRQ = 40,
    CRASH_SWI5_EGU5_IRQ = 41,
    CRASH_TIMER3_IRQ = 42,
    CRASH_TIMER4_IRQ = 43,
    CRASH_PWM0_IRQ = 44,
    CRASH_PDM_IRQ = 45,
    CRASH_MWU_IRQ = 48,
    CRASH_PWM1_IRQ = 49,
    CRASH_PWM2_IRQ = 50,
    CRASH_SPIM2_SPIS2_SPI2_IRQ = 51,
    CRASH_RTC2_IRQ = 52,
    CRASH_I2S_IRQ = 53,
    CRASH_FPU_IRQ = 54,
};

void crash(CrashMethod c)
{
	while(1)
		;
}

extern "C" {
	// Handlers for unhandled interrupts
	void NMI_Handler()                                  { crash(CRASH_NMI); }
	void HardFault_Handler()                            { crash(CRASH_HardFault); }
	void MemoryManagement_Handler()                     { crash(CRASH_MemoryManagement); }
	void BusFault_Handler()                             { crash(CRASH_BusFault); }
	void UsageFault_Handler()                           { crash(CRASH_UsageFault); }
	void SVC_Handler()                                  { crash(CRASH_SVC); }
	void DebugMon_Handler()                             { crash(CRASH_DebugMon); }
	void PendSV_Handler()                               { crash(CRASH_PendSV); }
	void POWER_CLOCK_IRQHandler()                       { crash(CRASH_POWER_CLOCK_IRQ); }
	void RADIO_IRQHandler()                             { crash(CRASH_RADIO_IRQ); }
	void UARTE0_UART0_IRQHandler()                      { crash(CRASH_UART0_IRQ); }
	void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler() { crash(CRASH_SPI0_IRQ); }
	void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler() { crash(CRASH_SPI1_IRQ); }
	void NFCT_IRQHandler()                              { crash(CRASH_NFCT_IRQ); }
	void GPIOTE_IRQHandler()                            { crash(CRASH_GPIOTE_IRQ); }
	void SAADC_IRQHandler()                             { crash(CRASH_SAADC_IRQ); }
	void TIMER0_IRQHandler()                            { crash(CRASH_TIMER0_IRQ); }
	//void TIMER1_IRQHandler()                            { crash(CRASH_TIMER1_IRQ); }
	//void TIMER2_IRQHandler()                            { crash(CRASH_TIMER2_IRQ); }
	void RTC0_IRQHandler()                              { crash(CRASH_RTC0_IRQ); }
	void TEMP_IRQHandler()                              { crash(CRASH_TEMP_IRQ); }
	void RNG_IRQHandler()                               { crash(CRASH_RNG_IRQ); }
	void ECB_IRQHandler()                               { crash(CRASH_ECB_IRQ); }
	void CCM_AAR_IRQHandler()                           { crash(CRASH_CCM_AAR_IRQ); }
	void WDT_IRQHandler()                               { crash(CRASH_WDT_IRQ); }
	//void RTC1_IRQHandler()                              { crash(CRASH_RTC1_IRQ); }
	void QDEC_IRQHandler()                              { crash(CRASH_QDEC_IRQ); }
	void COMP_LPCOMP_IRQHandler()                       { crash(CRASH_COMP_LPCOMP_IRQ); }
	void SWI0_EGU0_IRQHandler()                         { crash(CRASH_SWI0_EGU0_IRQ); }
	void SWI1_EGU1_IRQHandler()                         { crash(CRASH_SWI1_EGU1_IRQ); }
	//void SWI2_EGU2_IRQHandler()                         { crash(CRASH_SWI2_EGU2_IRQ); }
	void SWI3_EGU3_IRQHandler()                         { crash(CRASH_SWI3_EGU3_IRQ); }
	void SWI4_EGU4_IRQHandler()                         { crash(CRASH_SWI4_EGU4_IRQ); }
	void SWI5_EGU5_IRQHandler()                         { crash(CRASH_SWI5_EGU5_IRQ); }
	void TIMER3_IRQHandler()                            { crash(CRASH_TIMER3_IRQ); }
	void TIMER4_IRQHandler()                            { crash(CRASH_TIMER4_IRQ); }
	void PWM0_IRQHandler()                              { crash(CRASH_PWM0_IRQ); }
	void PDM_IRQHandler()                               { crash(CRASH_PDM_IRQ); }
	void MWU_IRQHandler()                               { crash(CRASH_MWU_IRQ); }
	void PWM1_IRQHandler()                              { crash(CRASH_PWM1_IRQ); }
	void PWM2_IRQHandler()                              { crash(CRASH_PWM2_IRQ); }
	void SPIM2_SPIS2_SPI2_IRQHandler()                  { crash(CRASH_SPIM2_SPIS2_SPI2_IRQ); }
	void RTC2_IRQHandler()                              { crash(CRASH_RTC2_IRQ); }
	void I2S_IRQHandler()                               { crash(CRASH_I2S_IRQ); }
	void FPU_IRQHandler()                               { crash(CRASH_FPU_IRQ); }
}
