module;

#include <chrono> //IntelliSense

module StepTimer;

namespace gfl
{
	void StepTimer::SetFixedTimeStep(bool isFixedTimestep)
	{
		this->isFixedTimeStep = isFixedTimestep;
	}

	void StepTimer::SetTargetElapsedTicks(uint64_t targetElapsed)
	{
		this->targetElapsedTicks = targetElapsed;
	}

	void StepTimer::SetTargetElapsedSeconds(double targetElapsed)
	{
		this->targetElapsedTicks = StepTimer::SecondsToTicks(targetElapsed);
	}

	uint64_t StepTimer::GetElapsedTicks() const
	{
		return this->elapsedTicks;
	}

	double StepTimer::GetElapsedSeconds() const
	{
		return StepTimer::TicksToSeconds(this->elapsedTicks);
	}

	uint64_t StepTimer::GetTotalTicks() const
	{
		return this->totalTicks;
	}

	double StepTimer::GetTotalSeconds() const
	{
		return StepTimer::TicksToSeconds(this->totalTicks);
	}

	uint32_t StepTimer::GetFrameCount() const
	{
		return this->frameCount;
	}

	uint32_t StepTimer::GetFramesPerSecond() const
	{
		return this->framesPerSecond;
	}

	int64_t StepTimer::GetTicks() const
	{
		return this->timer.GetTime<std::chrono::duration<int64_t, std::ratio<1, 10'000'000l>>>().count();
	}

	void StepTimer::Start()
	{
		this->timer.Start();
	}

	void StepTimer::Stop()
	{
		this->timer.Stop();
	}

	void StepTimer::Pause()
	{
		this->timer.Pause();
	}

	void StepTimer::Resume()
	{
		this->timer.Resume();
	}

	bool StepTimer::IsStarted() const
	{
		return this->timer.IsStarted();
	}

	bool StepTimer::IsPaused() const
	{
		return this->timer.IsPaused();
	}

	void StepTimer::ResetElapsedTime()
	{
		// After an intentional timing discontinuity (for instance a blocking IO operation) call this to avoid having the fixed timestep logic attempt a set of catch-up Update calls.
		this->lastTime = StepTimer::GetTicks();

		this->leftOverTicks = 0;
		this->framesPerSecond = 0;
		this->framesThisSecond = 0;
		this->secondCounter = 0;
	}

	void StepTimer::Tick(StepTimerNotify* stepTimerNotify)
	{
		const auto currentTime{StepTimer::GetTicks()};
		auto timeDelta{currentTime - this->lastTime};
		this->lastTime = currentTime;
		this->secondCounter += timeDelta;

		// Clamp excessively large time deltas (e.g. after paused in the debugger).
		static constexpr auto maxDelta{std::micro::den};
		if (timeDelta > maxDelta)
			timeDelta = maxDelta;

		const auto lastFrameCount{this->frameCount};
		if (this->isFixedTimeStep)
		{
			// Fixed timestep update logic.
			// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp the clock to exactly match the target value. This prevents tiny and irrelevant errors
			// from accumulating over time. Without this clamping, a game that requested a 60 fps fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
			// accumulate enough tiny errors that it would drop a frame. It is better to just round small deviations down to zero to leave things running smoothly.
			if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - this->targetElapsedTicks))) < StepTimer::TicksPerSecond / 4000)
				timeDelta = this->targetElapsedTicks;

			this->leftOverTicks += timeDelta;

			while (this->leftOverTicks >= this->targetElapsedTicks)
			{
				this->elapsedTicks = this->targetElapsedTicks;
				this->totalTicks += this->targetElapsedTicks;
				this->leftOverTicks -= this->targetElapsedTicks;
				this->frameCount++;

				stepTimerNotify->Update();
			}
		}
		else
		{
			// Variable timestep update logic.
			this->elapsedTicks = timeDelta;
			this->totalTicks += timeDelta;
			this->leftOverTicks = 0;
			this->frameCount++;

			stepTimerNotify->Update();
		}

		if (this->frameCount != lastFrameCount)
			this->framesThisSecond++;

		if (this->secondCounter >= StepTimer::TicksPerSecond)
		{
			this->framesPerSecond = this->framesThisSecond;
			this->framesThisSecond = 0;
			this->secondCounter %= StepTimer::TicksPerSecond;
		}
	}

	constexpr double StepTimer::TicksToSeconds(uint64_t ticks)
	{
		return static_cast<double>(ticks) / StepTimer::TicksPerSecond;
	}

	constexpr uint64_t StepTimer::SecondsToTicks(double seconds)
	{
		return static_cast<uint64_t>(seconds * StepTimer::TicksPerSecond);
	}
}