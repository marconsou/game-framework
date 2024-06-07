module;

#include <chrono>

export module Clock;

export namespace gfl
{
	class Clock
	{
	public:
		template<typename T = std::chrono::nanoseconds>
		static T GetTotalTime()
		{
			return std::chrono::duration_cast<T>(Clock::Now() - Clock::start);
		}

		static std::chrono::milliseconds GetTotalMilliseconds();
		static std::chrono::seconds GetTotalSeconds();
	private:
		static std::chrono::high_resolution_clock::time_point Now();
		static inline const std::chrono::high_resolution_clock::time_point start{Clock::Now()};
	};
}