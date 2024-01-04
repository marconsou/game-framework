module Log;

import <fstream>;
import DateTime;

namespace gfl
{
	void Log::Warning(std::string_view message)
	{
		Log::WriteLogFile("Warning", message);
	}

	void Log::Error(std::string_view message)
	{
		Log::WriteLogFile("Error", message);
		if (Log::onLogError)
			Log::onLogError(message);
	}

	void Log::OnLogError(std::function<void(std::string_view)> onLogError)
	{
		Log::onLogError = onLogError;
	}

	void Log::WriteLogFile(std::string_view type, std::string_view message)
	{
		std::ofstream file{"Log.txt", std::ios::app};
		if (file.is_open())
			file << std::format("[{} | {} | {}]: {}\n", DateTime::GetDate(), DateTime::GetTime(), type, message);
	}
}