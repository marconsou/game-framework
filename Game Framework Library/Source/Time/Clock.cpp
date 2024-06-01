module Clock;

import std;

namespace gfl
{
	std::chrono::milliseconds Clock::GetTotalMilliseconds()
	{
		return Clock::GetTotalTime<std::chrono::milliseconds>();
	}

	std::chrono::seconds Clock::GetTotalSeconds()
	{
		return Clock::GetTotalTime<std::chrono::seconds>();
	}

	std::chrono::high_resolution_clock::time_point Clock::Now()
	{
		return std::chrono::high_resolution_clock::now();
	}
}