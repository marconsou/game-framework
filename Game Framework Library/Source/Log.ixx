export module Log;

import <format>;
import <fstream>;
import <functional>;
import <string_view>;
import DateTime;

export namespace gfl
{
	class Log
	{
	public:
		static void Warning(std::string_view message)
		{
			Log::WriteLogFile("Warning", message);
		}

		static void Error(std::string_view message)
		{
			Log::WriteLogFile("Error", message);
			if (Log::onLogError)
				Log::onLogError(message);
		}

		static void OnLogError(std::function<void(std::string_view)> onLogError)
		{
			Log::onLogError = onLogError;
		}
	private:
		static inline std::function<void(std::string_view)> onLogError;

		static void WriteLogFile(std::string_view type, std::string_view message)
		{
			std::ofstream file{"Log.txt", std::ios::app};
			if (file.is_open())
				file << std::format("[{} | {} | {}]: {}\n", DateTime::GetDate(), DateTime::GetTime(), type, message);
		}
	};
}