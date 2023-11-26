export module WindowsApi;

import <memory>;
import <string>;
import <string_view>;
import "Windows.h";
import AppConfiguration;
import Log;

export namespace gfl
{
	class WindowsApi
	{
	public:
		static std::wstring GetExecutablePath()
		{
			auto length = 0;
			auto bufferLength = MAX_PATH;
			std::unique_ptr<wchar_t[]> path;
			do
			{
				bufferLength += 128;
				path = std::make_unique<wchar_t[]>(bufferLength);
				length = GetModuleFileName(nullptr, path.get(), bufferLength);
			} while (length == bufferLength);
			return std::wstring{path.get()};
		}

		static std::wstring_view GetExecutableId()
		{
			static const auto id{std::to_wstring(std::hash<std::wstring_view>{}(WindowsApi::GetExecutablePath()))};
			return id;
		}

		static const HWND GetHandleWindow()
		{
			return FindWindow(WindowsApi::GetExecutableId().data(), nullptr);
		}

		static std::pair<DWORD, RECT> GetWindowDimension(const AppConfiguration& appConfiguration)
		{
			DWORD windowStyle{};
			RECT windowRect{};

			windowRect.right = appConfiguration.Width;
			windowRect.bottom = appConfiguration.Height;

			if (appConfiguration.Windowed)
			{
				windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
				if (!AdjustWindowRectEx(&windowRect, windowStyle, false, 0))
					Log::Error("AdjustWindowRectEx");

				windowRect.right -= windowRect.left;
				windowRect.bottom -= windowRect.top;
				windowRect.left = (GetSystemMetrics(SM_CXSCREEN) - appConfiguration.Width) / 2;
				windowRect.top = (GetSystemMetrics(SM_CYSCREEN) - appConfiguration.Height) / 2;
			}
			return {windowStyle, windowRect};
		}

		static void RestoreApp()
		{
			if (const auto handleWindow = WindowsApi::GetHandleWindow())
			{
				ShowWindow(handleWindow, SW_RESTORE);
				SetForegroundWindow(handleWindow);
			}
		}
	};
}