module;

#include <chrono> //IntelliSense

module Timer;

import Clock;

namespace gfl
{
	Timer::Timer() : startTime{Clock::GetTotalMilliseconds()}
	{

	}

	int64_t Timer::GetMilliseconds() const
	{
		return this->GetTime().count();
	}

	double Timer::GetSeconds() const
	{
		return std::chrono::duration<double>(this->GetTime()).count();
	}

	bool Timer::IsPaused() const
	{
		return this->isPaused;
	}

	void Timer::Pause()
	{
		if (!this->isPaused)
			this->pauseTime = Clock::GetTotalMilliseconds() - this->startTime;

		this->isPaused = true;
	}

	void Timer::Resume()
	{
		if (this->isPaused)
			this->startTime = Clock::GetTotalMilliseconds() - this->pauseTime;

		this->isPaused = false;
	}

	std::chrono::milliseconds Timer::GetTime() const
	{
		return !this->isPaused ? (Clock::GetTotalMilliseconds() - this->startTime) : this->pauseTime;
	}
}