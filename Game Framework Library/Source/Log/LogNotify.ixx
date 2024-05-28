export module LogNotify;

import std;

export namespace gfl
{
	class LogNotify
	{
	public:
		LogNotify() = default;
		LogNotify(const LogNotify&) = delete;
		LogNotify(LogNotify&&) = delete;
		LogNotify& operator=(const LogNotify&) = delete;
		LogNotify& operator=(LogNotify&&) = delete;
		virtual ~LogNotify() = default;
		virtual void OnLogError(std::string_view message) const = 0;
	};
}