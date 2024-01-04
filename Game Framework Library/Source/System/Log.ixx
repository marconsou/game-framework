export module Log;

import <format>;
import <functional>;

export namespace gfl
{
	class Log
	{
	public:
		static void Warning(std::string_view message);
		static void Error(std::string_view message);
		static void OnLogError(std::function<void(std::string_view)> onLogError);
	private:
		static inline std::function<void(std::string_view)> onLogError;
		static void WriteLogFile(std::string_view type, std::string_view message);
	};
}