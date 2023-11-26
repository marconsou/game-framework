export module CrtDebugLogWindows;

import <crtdbg.h>;
import <stdlib.h>;

export namespace gfl
{
	class CrtDebugLogWindows
	{
	public:
		static void EnableCrtDebugLog()
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		}
	};
}