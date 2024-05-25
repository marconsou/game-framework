module;

#include <chrono> //IntelliSense

module Clock;

namespace gfl
{
	std::chrono::milliseconds Clock::GetTotalMilliseconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::GetTotalTime());
	}

	std::chrono::seconds Clock::GetTotalSeconds()
	{
		return std::chrono::duration_cast<std::chrono::seconds>(Clock::GetTotalTime());
	}

	std::chrono::nanoseconds Clock::GetTotalTime()
	{
		return Clock::Now() - Clock::start;
	}

	std::chrono::high_resolution_clock::time_point Clock::Now()
	{
		return std::chrono::high_resolution_clock::now();
	}
}