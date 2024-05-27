export module LogCallback;

import std;

export namespace gfl
{
	class LogCallback
	{
	public:
		LogCallback() = default;
		LogCallback(const LogCallback&) = delete;
		LogCallback(LogCallback&&) = delete;
		LogCallback& operator=(const LogCallback&) = delete;
		LogCallback& operator=(LogCallback&&) = delete;
		virtual ~LogCallback() = default;
		virtual void OnLogError(std::string_view message) const = 0;
	};
}