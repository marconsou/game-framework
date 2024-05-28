export module FileLogger;

import Log;
import LogNotify;

export namespace gfl
{
	class FileLogger : public Log
	{
	public:
		FileLogger(const LogNotify* logNotify = nullptr);
		void Warning(std::string_view message) override;
		void Error(std::string_view message) override;
	private:
		const LogNotify* logNotify{};
		void WriteData(std::string_view type, std::string_view message);
	};
}