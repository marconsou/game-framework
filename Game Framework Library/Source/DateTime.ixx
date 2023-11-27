module;

#include <chrono>

export module DateTime;

import <format>;

export namespace gfl
{
	class DateTime
	{
	public:
		static std::string GetDate()
		{
			const auto now = std::chrono::system_clock::now();
			const auto ymd = std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(now)};
			return std::format("{}-{:0>2}-{:0>2}", static_cast<int>(ymd.year()), unsigned{ymd.month()}, unsigned{ymd.day()});
		}

		static std::string GetTime()
		{
			const auto now = std::chrono::system_clock::now();
			const auto hms = std::chrono::hh_mm_ss{std::chrono::floor<std::chrono::seconds>(now - std::chrono::floor<std::chrono::days>(now))};
			return std::format("{:0>2}:{:0>2}:{:0>2}", hms.hours().count(), hms.minutes().count(), hms.seconds().count());
		}
	};
}