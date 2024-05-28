module;

#include "WindowsCompact.h" //IntelliSense

module WindowsMutex;

import WindowsApi;

namespace gfl
{
	WindowsMutex::WindowsMutex() :
		handle{CreateMutex(nullptr, true, WindowsApi::GetExecutableId().data())},
		isAlreadyRunning{(GetLastError() == ERROR_ALREADY_EXISTS)}
	{

	}

	WindowsMutex::~WindowsMutex()
	{
		if (this->handle)
		{
			if (this->IsFirstInstance())
				ReleaseMutex(this->handle);

			CloseHandle(this->handle);
		}
	}

	bool WindowsMutex::IsFirstInstance() const
	{
		return !this->isAlreadyRunning;
	}
}