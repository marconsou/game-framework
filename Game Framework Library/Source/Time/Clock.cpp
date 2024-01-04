module Clock;

namespace gfl
{
	std::chrono::nanoseconds Clock::CurrentTime()
	{
		return Clock::Now() - Clock::start;
	}

	std::chrono::steady_clock::time_point Clock::Now()
	{
		return std::chrono::high_resolution_clock::now();
	}
}