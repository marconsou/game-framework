export module StepTimer;

import std;
import Clock;
import StepTimerNotify;

export namespace gfl
{
	class StepTimer
	{
	public:
		void SetFixedTimeStep(bool isFixedTimestep);
		void SetTargetElapsedTicks(std::uint64_t targetElapsed);
		void SetTargetElapsedSeconds(double targetElapsed);
		std::uint64_t GetElapsedTicks() const;
		double GetElapsedSeconds() const;
		std::uint64_t GetTotalTicks() const;
		double GetTotalSeconds() const;
		std::uint32_t GetFrameCount() const;
		std::uint32_t GetFramesPerSecond() const;
		void ResetElapsedTime();
		void Tick(StepTimerNotify* stepTimerNotify);
	private:
		bool isFixedTimeStep{};
		std::int64_t lastTime{StepTimer::GetTicks()};
		std::uint64_t targetElapsedTicks{StepTimer::TicksPerSecond / 60};
		std::uint64_t elapsedTicks;
		std::uint64_t totalTicks;
		std::uint64_t leftOverTicks;
		std::uint32_t frameCount;
		std::uint32_t framesPerSecond;
		std::uint32_t framesThisSecond;
		std::uint64_t secondCounter;
		static constexpr std::uint64_t TicksPerSecond{10'000'000};
		static std::int64_t GetTicks();
		static constexpr double TicksToSeconds(std::uint64_t ticks);
		static constexpr std::uint64_t SecondsToTicks(double seconds);
	};
}