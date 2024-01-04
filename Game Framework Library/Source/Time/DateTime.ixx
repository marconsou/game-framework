export module DateTime;

import <string>;

export namespace gfl
{
	class DateTime
	{
	public:
		static std::string GetDate();
		static std::string GetTime();
	};
}