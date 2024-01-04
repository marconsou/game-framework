export module Timer;

import <chrono>;

export namespace gfl
{
	class Timer
	{
	public:
		Timer();
		std::uint64_t GetTicks() const;
		double GetSeconds() const;
		bool IsPaused() const;
		void Pause();
		void Resume();
	private:
		bool isPaused{};
		std::chrono::nanoseconds startTime{};
		std::chrono::nanoseconds pauseTime{};
		std::chrono::nanoseconds GetTime() const;
	};
}