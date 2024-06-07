module;

#include "WindowsCompact.h"

export module WindowsMutex;

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