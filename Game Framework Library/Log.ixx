export module Log;

import std;

export namespace gfl
{
	class Log
	{
	public:
		virtual void Warning(std::string_view message) = 0;
		virtual void Error(std::string_view message) = 0;
	};
}