export module Log;

import std;

export namespace gfl
{
	class Log
	{
	public:
		Log() = default;
		Log(const Log&) = delete;
		Log(Log&&) = delete;
		Log& operator=(const Log&) = delete;
		Log& operator=(Log&&) = delete;
		virtual ~Log() = default;
		virtual void Warning(std::string_view message) = 0;
		virtual void Error(std::string_view message) = 0;
	};
}