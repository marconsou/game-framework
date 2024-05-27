export module File;

import std;

export namespace gfl
{
	class File
	{
	public:
		static std::string ExtractData(std::string_view fileName);
	};
}