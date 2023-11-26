export module MutexWindows;

import "Windows.h";
import Log;
import WindowsApi;

export namespace gfl
{
	class MutexWindows
	{
	public:
		MutexWindows() :
			handle{CreateMutex(nullptr, true, WindowsApi::GetExecutableId().data())},
			isAlreadyRunning{(GetLastError() == ERROR_ALREADY_EXISTS)}
		{

		}

		~MutexWindows()
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

		bool IsFirstInstance() const
		{
			return !this->isAlreadyRunning;
		}
	private:
		const HANDLE handle{};
		const bool isAlreadyRunning{};
	};
}