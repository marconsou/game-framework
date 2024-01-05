module CrtDebugLogWindows;

import <crtdbg.h>;

namespace gfl
{
	void CrtDebugLogWindows::EnableCrtDebugLog()
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}
}