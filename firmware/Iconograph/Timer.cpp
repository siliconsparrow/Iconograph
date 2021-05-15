/*
 * Timer.cpp
 *
 *  Created on: 9 May 2021
 *      Author: adam
 */

#include "Timer.h"
#include "nrf_timer.h"
#include "nrf_nvic.h"
#include "board.h"

// Definitions for the timer peripheral.
// You can override these in board.h if you like.

#ifndef TIMER_DEVICE
	#define TIMER_DEVICE         NRF_TIMER1
	#define TIMER_HANDLER        TIMER1_IRQHandler
	#define TIMER_IRQN           TIMER1_IRQn
#endif

#ifndef TIMER_INTERRUPT_FREQ
	#define TIMER_INTERRUPT_FREQ 100 // Hz
#endif

#ifndef TIMER_IRQ_PRIORITY
	#define TIMER_IRQ_PRIORITY   3
#endif

// Don't change these definitions.
#define TIMER_CMP_CHAN           0 // CC channel to use for interrupt generation.
#define TIMER_CAP_CHAN           1 // CC channel to use for directly reading the counter.
#define TIMER_BASEFREQ           125000
#define TIMER_INTERRUPT_INTERVAL (TIMER_BASEFREQ / TIMER_INTERRUPT_FREQ)
#define TIMER_US_PER_TICK        (1000000 / TIMER_BASEFREQ)
#define TIMER_MS_PER_TICK        (1000 / TIMER_INTERRUPT_FREQ)

Timer *Timer::instance()
{
	static Timer *g_timer = 0;

	if(g_timer == 0) {
		g_timer = new Timer();
	}

	return g_timer;
}

Timer::Timer()
{
    // Set up timer.
	TIMER_DEVICE->TASKS_STOP  = 1;                           // Stop timer when changing settings.
	TIMER_DEVICE->BITMODE     = TIMER_BITMODE_BITMODE_16Bit;
	TIMER_DEVICE->PRESCALER   = NRF_TIMER_FREQ_125kHz;       // Set timebase.
	TIMER_DEVICE->MODE        = TIMER_MODE_MODE_Timer;       // Set timer (not counter) mode.
	TIMER_DEVICE->TASKS_CLEAR = 1;                           // Start counting from 0.

    // Start the timer.
	TIMER_DEVICE->TASKS_START = 1;

    // Set an interrupt to service tasks.
	TIMER_DEVICE->EVENTS_COMPARE[TIMER_CMP_CHAN] = 0;
	TIMER_DEVICE->CC[TIMER_CMP_CHAN] = TIMER_INTERRUPT_INTERVAL;
	TIMER_DEVICE->INTENSET = ((1 << TIMER_CMP_CHAN) << 16);

    // Enable interrupts.
	sd_nvic_SetPriority(TIMER_IRQN, TIMER_IRQ_PRIORITY);
	sd_nvic_EnableIRQ(TIMER_IRQN);
}

int Timer::setPeriodic(unsigned msInterval, Delegate *delegate, unsigned param)
{
	for(int i = 0; i < kMaxPeriodicTimers; i++) {
		if(_periodic[i].isFree()) {
			_periodic[i].set(msInterval, delegate, param);
			return i;
		}
	}

	return -1;
}

void Timer::irq()
{
	for(int i = 0; i < kMaxPeriodicTimers; i++) {
		_periodic[i].addTime(TIMER_MS_PER_TICK);
	}
}

extern "C" void TIMER_HANDLER()
{
	if (0 != TIMER_DEVICE->EVENTS_COMPARE[TIMER_CMP_CHAN]) {
		// Acknowledge the interrupt.
		TIMER_DEVICE->EVENTS_COMPARE[TIMER_CMP_CHAN] = 0;

		// Set new interrupt based on the previous interrupt time.
		TIMER_DEVICE->CC[TIMER_CMP_CHAN] += TIMER_INTERRUPT_INTERVAL;

		// Do stuff.
		Timer::instance()->irq();
	}
}

Timer::Periodic::Periodic()
	: _delegate(0)
	, _interval(0)
	, _param(0)
	, _elapsed(0)
{ }

bool Timer::Periodic::isFree() const
{
	return _delegate == 0;
}

void Timer::Periodic::set(unsigned msInterval, Delegate *delegate, unsigned param)
{
	_delegate = delegate;
	_elapsed  = 0;
	_interval = msInterval;
	_param    = param;
}

void Timer::Periodic::clear()
{
	_delegate = 0;
}

void Timer::Periodic::addTime(unsigned ms)
{
	if(0 != _delegate) {
		_elapsed += ms;
		if(_elapsed >= _interval) {
			_elapsed -= _interval;
			_delegate->evtTimer(_param);
		}
	}
}
