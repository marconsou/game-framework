module Timer;

import Clock;

namespace gfl
{
	Timer::Timer() : startTime{Clock::CurrentTime()}
	{

	}

	std::uint64_t Timer::GetTicks() const
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
			this->pauseTime = Clock::CurrentTime() - this->startTime;

		this->isPaused = true;
	}

	void Timer::Resume()
	{
		if (this->isPaused)
			this->startTime = Clock::CurrentTime() - this->pauseTime;

		this->isPaused = false;
	}

	std::chrono::nanoseconds Timer::GetTime() const
	{
		return !this->isPaused ? (Clock::CurrentTime() - this->startTime) : this->pauseTime;
	}
}