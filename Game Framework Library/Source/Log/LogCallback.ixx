export module LogCallback;

import std;

export namespace gfl
{
	class LogCallback
	{
	public:
		virtual void OnLogError(std::string_view message) = 0;
	};
}