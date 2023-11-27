module;

#include <chrono>

export module Clock;

export namespace gfl
{
	class Clock
	{
	public:
		static std::chrono::nanoseconds CurrentTime()
		{
			return Clock::Now() - Clock::start;
		}
	private:
		static std::chrono::steady_clock::time_point Now()
		{
			return std::chrono::high_resolution_clock::now();
		}

		static inline const std::chrono::steady_clock::time_point start{Clock::Now()};
	};
}