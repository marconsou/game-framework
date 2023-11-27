export module StepTimer;

import <functional>;
import Timer;

export namespace gfl
{
	class StepTimer
	{
	public:
		StepTimer() : lastTime{this->timer.GetTicks()}
		{
			this->SetFixedTimeStep(true);
			this->SetTargetElapsedSeconds(1.0 / 60.0);
		}

		uint64_t GetElapsedTicks() const
		{
			return this->elapsedTicks;
		}

		double GetElapsedSeconds() const
		{
			return StepTimer::TicksToSeconds(this->elapsedTicks);
		}

		uint64_t GetTotalTicks() const
		{
			return this->totalTicks;
		}

		double GetTotalSeconds() const
		{
			return StepTimer::TicksToSeconds(this->totalTicks);
		}

		uint32_t GetFrameCount() const
		{
			return this->frameCount;
		}

		uint32_t GetFramesPerSecond() const
		{
			return this->framesPerSecond;
		}

		void SetFixedTimeStep(bool isFixedTimestep)
		{
			this->isFixedTimeStep = isFixedTimestep;
		}

		void SetTargetElapsedTicks(uint64_t targetElapsed)
		{
			this->targetElapsedTicks = targetElapsed;
		}

		void SetTargetElapsedSeconds(double targetElapsed)
		{
			this->targetElapsedTicks = StepTimer::SecondsToTicks(targetElapsed);
		}

		bool IsPaused() const
		{
			return this->timer.IsPaused();
		}

		void Pause()
		{
			this->timer.Pause();
		}

		void Resume()
		{
			this->timer.Resume();
		}

		void Tick(std::function<void()> update)
		{
			const auto timeDelta = this->CalculateTimeDelta();
			const auto lastFrameCount = this->frameCount;
			this->FixedTimeStep(update, timeDelta);
			this->CalculateFramesPerSecond(lastFrameCount);
		}

		void ResetElapsedTime()
		{
			// After an intentional timing discontinuity (for instance a blocking IO operation) call this to avoid having the fixed timestep logic attempt a set of catch-up Update calls.
			this->leftOverTicks = 0;
			this->framesPerSecond = 0;
			this->framesThisSecond = 0;
			this->secondCounter = 0;
		}
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

		uint64_t CalculateTimeDelta()
		{
			const auto currentTime = this->timer.GetTicks();
			auto timeDelta = (currentTime - this->lastTime) / 100;

			this->lastTime = currentTime;
			this->secondCounter += timeDelta;

			// Clamp excessively large time deltas (e.g. after paused in the debugger).
			static constexpr auto maxDelta = StepTimer::TicksPerSecond / 10;
			if (timeDelta > maxDelta)
				timeDelta = maxDelta;

			return timeDelta;
		}

		void FixedTimeStep(std::function<void()> update, uint64_t timeDelta)
		{
			if (this->isFixedTimeStep)
			{
				// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp the clock to exactly match the target value.
				// This prevents tiny and irrelevant errors from accumulating over time.
				// Without this clamping, a game that requested a 60 fps fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually accumulate enough tiny errors that it would drop a frame.
				// It is better to just round small deviations down to zero to leave things running smoothly.
				if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - this->targetElapsedTicks))) < StepTimer::TicksPerSecond / 4000)
					timeDelta = this->targetElapsedTicks;

				this->leftOverTicks += timeDelta;

				while (this->leftOverTicks >= this->targetElapsedTicks)
				{
					this->elapsedTicks = this->targetElapsedTicks;
					this->totalTicks += this->targetElapsedTicks;
					this->leftOverTicks -= this->targetElapsedTicks;
					this->frameCount++;

					update();
				}
			}
			else
			{
				// Variable timestep update logic.
				this->elapsedTicks = timeDelta;
				this->totalTicks += timeDelta;
				this->leftOverTicks = 0;
				this->frameCount++;

				update();
			}
		}

		void CalculateFramesPerSecond(uint32_t lastFrameCount)
		{
			if (this->frameCount != lastFrameCount)
				this->framesThisSecond++;

			if (this->secondCounter >= StepTimer::TicksPerSecond)
			{
				this->framesPerSecond = this->framesThisSecond;
				this->framesThisSecond = 0;
				this->secondCounter %= StepTimer::TicksPerSecond;
			}
		}

		static constexpr uint64_t TicksPerSecond = 10'000'000;

		static double TicksToSeconds(uint64_t ticks)
		{
			return static_cast<double>(ticks) / StepTimer::TicksPerSecond;
		}

		static uint64_t SecondsToTicks(double seconds)
		{
			return static_cast<uint64_t>(seconds * StepTimer::TicksPerSecond);
		}
	};
}