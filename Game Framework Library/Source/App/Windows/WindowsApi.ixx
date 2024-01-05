export module WindowsApi;

import <string_view>;
import "WindowsCompact.h";
import AppConfiguration;

export namespace gfl
{
	class WindowsApi
	{
	public:
		static std::wstring GetExecutablePath();
		static std::wstring_view GetExecutableId();
		static const HWND GetHandleWindow();
		static std::pair<DWORD, RECT> GetWindowDimension(const AppConfiguration& appConfiguration);
		static void RestoreApp();
	};
}