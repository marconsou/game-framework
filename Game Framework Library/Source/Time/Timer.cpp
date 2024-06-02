module;

#include <chrono> //IntelliSense

module Timer;

import Clock;

namespace gfl
{
	std::chrono::milliseconds Timer::GetMilliseconds() const
	{
		return this->GetTime<std::chrono::milliseconds>();
	}

	std::chrono::seconds Timer::GetSeconds() const
	{
		return this->GetTime<std::chrono::seconds>();
	}

	void Timer::Start()
	{
		this->started = true;
		this->paused = false;
		this->startTime = Clock::GetTotalTime();
		this->pauseTime = std::chrono::nanoseconds::zero();
	}

	void Timer::Stop()
	{
		this->started = false;
		this->paused = false;
		this->startTime = std::chrono::nanoseconds::zero();
		this->pauseTime = std::chrono::nanoseconds::zero();
	}

	void Timer::Pause()
	{
		if (this->started && !this->paused)
		{
			this->paused = true;
			this->pauseTime = Clock::GetTotalTime() - this->startTime;
			this->startTime = std::chrono::nanoseconds::zero();
		}
	}

	void Timer::Resume()
	{
		if (this->started && this->paused)
		{
			this->paused = false;
			this->startTime = Clock::GetTotalTime() - this->pauseTime;
			this->pauseTime = std::chrono::nanoseconds::zero();
		}
	}

	bool Timer::IsStarted() const
	{
		return this->started;
	}

	bool Timer::IsPaused() const
	{
		return this->paused && this->started;
	}
}