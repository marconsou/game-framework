export module Timer;

import std;

export namespace gfl
{
	class Timer
	{
	public:
		Timer();
		std::int64_t GetMilliseconds() const;
		double GetSeconds() const;
		bool IsPaused() const;
		void Pause();
		void Resume();
	private:
		bool isPaused{};
		std::chrono::milliseconds startTime{};
		std::chrono::milliseconds pauseTime{};
		std::chrono::milliseconds GetTime() const;
	};
}