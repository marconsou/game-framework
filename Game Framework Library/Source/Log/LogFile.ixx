export module LogFile;

import Log;
import LogCallback;

export namespace gfl
{
	class LogFile : public Log
	{
	public:
		LogFile(const LogCallback* logCallback = nullptr);
		void Warning(std::string_view message) override;
		void Error(std::string_view message) override;
	private:
		const LogCallback* logCallback{};
		void WriteData(std::string_view type, std::string_view message);
	};
}