module;

#include <chrono> //IntelliSense

module DateTime;

import std;

namespace gfl
{
	DateTime::DateTime()
	{
#ifdef _DEBUG
		//https://github.com/microsoft/STL/issues/2504
		static constinit bool registerOnce{};
		if (!registerOnce)
		{
			atexit([]() { std::chrono::get_tzdb_list().~tzdb_list(); });
			registerOnce = true;
		}
#endif
	}

	std::string DateTime::GetDate() const
	{
		return std::format("{:%Y-%m-%d}", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()});
	}

	std::string DateTime::GetTime() const
	{
		return std::format("{:%H:%M:%OS}", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()});
	}
}