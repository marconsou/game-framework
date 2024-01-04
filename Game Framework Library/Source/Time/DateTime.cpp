module DateTime;

import <chrono>;
import <format>;

//https://github.com/microsoft/STL/issues/2504
static void _RegisterFunction_()
{
#ifdef _DEBUG
	static constinit bool registerOnce = false;
	if (!registerOnce)
	{
		atexit([]()
		{
			std::chrono::get_tzdb_list().~tzdb_list();
		});
		registerOnce = true;
	}
#endif
}

namespace gfl
{
	std::string DateTime::GetDate()
	{
		_RegisterFunction_();
		return std::format("{:%Y-%m-%d}", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()});
	}

	std::string DateTime::GetTime()
	{
		_RegisterFunction_();
		return std::format("{:%H:%M:%OS}", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()});
	}
}