module FileLogger;

import DateTime;

namespace gfl
{
	FileLogger::FileLogger(const LogCallback* logCallback) : logCallback{logCallback}
	{

	}

	void FileLogger::Warning(std::string_view message)
	{
		this->WriteData("Warning", message);
	}

	void FileLogger::Error(std::string_view message)
	{
		this->WriteData("Error", message);
		if (this->logCallback)
			this->logCallback->OnLogError(message);
	}

	void FileLogger::WriteData(std::string_view type, std::string_view message)
	{
		std::ofstream file{"Log.txt", std::ios::app};
		if (file.is_open())
			file << std::format("[{} | {} | {}]: {}\n", DateTime::GetDate(), DateTime::GetTime(), type, message);
	}
}