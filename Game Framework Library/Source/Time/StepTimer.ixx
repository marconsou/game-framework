export module StepTimer;

import <functional>;
import Timer;

export namespace gfl
{
	class StepTimer
	{
	public:
		StepTimer();
		uint64_t GetElapsedTicks() const;
		double GetElapsedSeconds() const;
		uint64_t GetTotalTicks() const;
		double GetTotalSeconds() const;
		uint32_t GetFrameCount() const;
		uint32_t GetFramesPerSecond() const;
		void SetFixedTimeStep(bool isFixedTimestep);
		void SetTargetElapsedTicks(uint64_t targetElapsed);
		void SetTargetElapsedSeconds(double targetElapsed);
		bool IsPaused() const;
		void Pause();
		void Resume();
		void Tick(std::function<void()> update);
		void ResetElapsedTime();
	private:
		bool isFixedTimeStep{};
		uint64_t targetElapsedTicks{};
		uint64_t lastTime{};
		uint64_t elapsedTicks{};
		uint64_t totalTicks{};
		uint64_t leftOverTicks{};
		uint32_t frameCount{};
		uint32_t framesPerSecond{};
		uint32_t framesThisSecond{};
		uint64_t secondCounter{};
		Timer timer;
		static constexpr uint64_t ticksPerSecond = 10'000'000;
		uint64_t CalculateTimeDelta();
		void FixedTimeStep(std::function<void()> update, uint64_t timeDelta);
		void CalculateFramesPerSecond(uint32_t lastFrameCount);
		static double TicksToSeconds(uint64_t ticks);
		static uint64_t SecondsToTicks(double seconds);
	};
}