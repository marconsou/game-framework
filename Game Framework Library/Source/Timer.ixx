module;

#include <chrono>

export module Timer;

import Clock;

export namespace gfl
{
	class Timer
	{
	public:
		Timer() : startTime{Clock::CurrentTime()}
		{

		}

		std::uint64_t GetTicks() const
		{
			return this->GetTime().count();
		}

		double GetSeconds() const
		{
			return std::chrono::duration<double>(this->GetTime()).count();
		}

		bool IsPaused() const
		{
			return this->isPaused;
		}

		void Pause()
		{
			if (!this->isPaused)
				this->pauseTime = Clock::CurrentTime() - this->startTime;

			this->isPaused = true;
		}

		void Resume()
		{
			if (this->isPaused)
				this->startTime = Clock::CurrentTime() - this->pauseTime;

			this->isPaused = false;
		}
	private:
		bool isPaused{};
		std::chrono::nanoseconds startTime{};
		std::chrono::nanoseconds pauseTime{};

		std::chrono::nanoseconds GetTime() const
		{
			return !this->isPaused ? (Clock::CurrentTime() - this->startTime) : this->pauseTime;
		}
	};
}