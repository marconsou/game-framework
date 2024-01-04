export module Clock;

import <chrono>;

export namespace gfl
{
	class Clock
	{
	public:
		static std::chrono::nanoseconds CurrentTime();
	private:
		static std::chrono::steady_clock::time_point Now();
		static inline const std::chrono::steady_clock::time_point start{Clock::Now()};
	};
}