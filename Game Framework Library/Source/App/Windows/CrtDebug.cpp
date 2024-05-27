module;

#include "CrtDebug.h" //IntelliSense

module CrtDebug;

namespace gfl
{
	void CrtDebug::EnableLog()
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}
}