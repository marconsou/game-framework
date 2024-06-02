export module Timer;

import std;
import Clock;

export namespace gfl
{
	class Timer
	{
	public:
		template<typename T = std::chrono::nanoseconds>
		T GetTime() const
		{
			if (this->started)
			{
				if (this->paused)
					return std::chrono::duration_cast<T>(this->pauseTime);
				else
					return std::chrono::duration_cast<T>(Clock::GetTotalTime() - this->startTime);
			}
			return T::zero();
		}

		std::chrono::milliseconds GetMilliseconds() const;
		std::chrono::seconds GetSeconds() const;
		void Start();
		void Stop();
		void Pause();
		void Resume();
		bool IsStarted() const;
		bool IsPaused() const;
	private:
		bool started{};
		bool paused{};
		std::chrono::nanoseconds startTime{};
		std::chrono::nanoseconds pauseTime{};
	};
}