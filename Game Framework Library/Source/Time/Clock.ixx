export module Clock;

import std;

export namespace gfl
{
	class Clock
	{
	public:
		static std::chrono::milliseconds GetTotalMilliseconds();
		static std::chrono::seconds GetTotalSeconds();
	private:
		static std::chrono::nanoseconds GetTotalTime();
		static std::chrono::high_resolution_clock::time_point Now();
		static inline const std::chrono::high_resolution_clock::time_point start{Clock::Now()};
	};
}