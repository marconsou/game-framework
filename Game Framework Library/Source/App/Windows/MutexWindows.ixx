export module MutexWindows;

import "WindowsCompact.h";

export namespace gfl
{
	class MutexWindows
	{
	public:
		MutexWindows();
		~MutexWindows();
		bool IsFirstInstance() const;
	private:
		const HANDLE handle{};
		const bool isAlreadyRunning{};
	};
}