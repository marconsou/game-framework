module MutexWindows;

import "WindowsCompact.h";
import Log;
import WindowsApi;

namespace gfl
{
	MutexWindows::MutexWindows() :
		handle{CreateMutex(nullptr, true, WindowsApi::GetExecutableId().data())},
		isAlreadyRunning{(GetLastError() == ERROR_ALREADY_EXISTS)}
	{

	}

	MutexWindows::~MutexWindows()
	{
		if (this->handle)
		{
			if (this->IsFirstInstance())
			{
				if (!ReleaseMutex(this->handle))
					Log::Error("ReleaseMutex");
			}

			if (!CloseHandle(this->handle))
				Log::Error("CloseHandle");
		}
	}

	bool MutexWindows::IsFirstInstance() const
	{
		return !this->isAlreadyRunning;
	}
}