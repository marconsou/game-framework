export module File;

import <fstream>;

export namespace gfl
{
	class File
	{
	public:
		static std::string ExtractData(std::string_view fileName);
	};
}