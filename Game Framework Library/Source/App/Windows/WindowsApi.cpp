module;

#include "WindowsCompact.h" //IntelliSense

module WindowsApi;

namespace gfl
{
	std::wstring WindowsApi::GetExecutablePath()
	{
		auto length{0};
		auto bufferLength{MAX_PATH};
		std::unique_ptr<wchar_t[]> path;
		do
		{
			bufferLength += 128;
			path = std::make_unique<wchar_t[]>(bufferLength);
			length = GetModuleFileName(nullptr, path.get(), bufferLength);
		} while (length == bufferLength);
		return std::wstring{path.get()};
	}

	std::wstring_view WindowsApi::GetExecutableId()
	{
		static const auto id{std::to_wstring(std::hash<std::wstring_view>{}(WindowsApi::GetExecutablePath()))};
		return id;
	}

	const HWND WindowsApi::GetHandleWindow()
	{
		return FindWindow(WindowsApi::GetExecutableId().data(), nullptr);
	}

	std::optional<std::pair<DWORD, RECT>> WindowsApi::GetWindowDimension(const AppConfiguration& appConfiguration)
	{
		DWORD windowStyle{};
		RECT windowRect{};

		windowRect.right = appConfiguration.Width;
		windowRect.bottom = appConfiguration.Height;

		if (appConfiguration.Windowed)
		{
			windowStyle = !appConfiguration.Resize ? (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) : WS_OVERLAPPEDWINDOW;
			if (AdjustWindowRectEx(&windowRect, windowStyle, false, 0))
			{
				windowRect.right -= windowRect.left;
				windowRect.bottom -= windowRect.top;
				windowRect.left = (GetSystemMetrics(SM_CXSCREEN) - appConfiguration.Width) / 2;
				windowRect.top = (GetSystemMetrics(SM_CYSCREEN) - appConfiguration.Height) / 2;
			}
			else
				return std::nullopt;
		}
		return std::pair<DWORD, RECT>{windowStyle, windowRect};
	}

	void WindowsApi::RestoreApp()
	{
		if (const auto handleWindow = WindowsApi::GetHandleWindow())
		{
			ShowWindow(handleWindow, SW_RESTORE);
			SetForegroundWindow(handleWindow);
		}
	}

	bool WindowsApi::Succeeded(HRESULT result)
	{
		return SUCCEEDED(result);
	}

	bool WindowsApi::Failed(HRESULT result)
	{
		return FAILED(result);
	}
}