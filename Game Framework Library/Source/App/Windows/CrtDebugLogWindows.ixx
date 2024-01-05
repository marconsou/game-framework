export module CrtDebugLogWindows;

import <crtdbg.h>;

export namespace gfl
{
	class CrtDebugLogWindows
	{
	public:
		static void EnableCrtDebugLog();
	};
}