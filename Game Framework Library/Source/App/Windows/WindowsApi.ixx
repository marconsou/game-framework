export module WindowsApi;

import std;
import AppConfiguration;
import "WindowsCompact.h";

export namespace gfl
{
	class WindowsApi
	{
	public:
		static std::wstring GetExecutablePath();
		static std::wstring_view GetExecutableId();
		static const HWND GetHandleWindow();
		static std::optional<std::pair<DWORD, RECT>> GetWindowDimension(const AppConfiguration& appConfiguration);
		static void RestoreApp();
	};
}