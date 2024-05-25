export module DateTime;

import std;

export namespace gfl
{
	class DateTime
	{
	public:
		DateTime();
		std::string GetDate() const;
		std::string GetTime() const;
	};
}