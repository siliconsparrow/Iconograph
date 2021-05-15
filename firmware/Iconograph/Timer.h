/*
 * Timer.h
 *
 *  Created on: 9 May 2021
 *      Author: adam
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
	class Delegate
	{
	public:
		virtual ~Delegate() { }

		virtual void evtTimer(unsigned param) = 0;
	};

	static Timer *instance();

	Timer();

	int setPeriodic(unsigned msInterval, Delegate *delegate, unsigned param = 0);

private:
	enum { kMaxPeriodicTimers = 4 };

	class Periodic
	{
	public:
		Periodic();
		bool isFree() const;
		void set(unsigned msInterval, Delegate *delegate, unsigned param);
		void clear();
		void addTime(unsigned ms);

	private:
		Timer::Delegate *_delegate;
		unsigned         _interval;
		unsigned         _param;
		unsigned         _elapsed;
	};

	Periodic _periodic[kMaxPeriodicTimers];

public:
	void irq();
};

#endif // TIMER_H_
