export module DateTime;

import <time.h>;
import <format>;

export namespace gfl
{
	class DateTime
	{
	public:
		static std::string GetDate()
		{
			if (const auto localTime = DateTime::GetLocalTime())
				return std::format("{:0>2}-{:0>2}-{:0>2}", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday);
			return {};
		}

		static std::string GetTime()
		{
			if (const auto localTime = DateTime::GetLocalTime())
				return std::format("{:0>2}:{:0>2}:{:0>2}", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
			return {};
		}
	private:
		static tm* GetLocalTime()
		{
			static tm timeData{};
			const auto time = _time64(nullptr);
			if (!_localtime64_s(&timeData, &time))
				return &timeData;
			return nullptr;
		}
	};
}