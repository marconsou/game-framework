export module File;

import <fstream>;
import <string_view>;
import Log;

export namespace gfl
{
	class File
	{
	public:
		static std::string ExtractData(std::string_view fileName)
		{
			std::string buffer;
			std::ifstream file{fileName.data(), std::ios::in | std::ios::binary};
			if (file.is_open())
			{
				file.seekg(0, std::ios::end);
				buffer.resize(static_cast<size_t>(file.tellg()));
				file.seekg(0, std::ios::beg);
				file.read(&buffer[0], buffer.size());
			}
			return buffer;
		}
	};
}