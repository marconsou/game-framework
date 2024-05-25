export module DateTime;

import std;

export namespace gfl
{
	class DateTime
	{
	public:
		static std::string GetDate();
		static std::string GetTime();
	private:
		static std::string FormatData(std::string_view format);
	};
}