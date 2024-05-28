export module WindowsMutex;

import "WindowsCompact.h";

export namespace gfl
{
	class WindowsMutex
	{
	public:
		WindowsMutex();
		~WindowsMutex();
		bool IsFirstInstance() const;
	private:
		const HANDLE handle{};
		const bool isAlreadyRunning{};
	};
}