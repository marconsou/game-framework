module;

#include <chrono> //IntelliSense

module DateTime;

import std;

namespace gfl
{
	std::string DateTime::GetDate()
	{
		return DateTime::FormatData("{:%Y-%m-%d}");
	}

	std::string DateTime::GetTime()
	{
		return DateTime::FormatData("{:%H:%M:%OS}");
	}

	std::string DateTime::FormatData(std::string_view format)
	{
#ifdef _DEBUG
		//https://github.com/microsoft/STL/issues/2504
		static constinit bool registerOnce{};
		if (!registerOnce)
		{
			atexit([]()
			{
#pragma warning( push )
#pragma warning( disable : 26457)
				std::chrono::get_tzdb_list().~tzdb_list();
#pragma warning( pop ) 
			});
			registerOnce = true;
		}
#endif
		const auto params = std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()};
		return std::vformat(format, std::make_format_args(params));
	}
}