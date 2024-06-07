module;

#include "WindowsCompact.h"

module WindowsApp;

import WindowsApi;
import WindowsKeyboard;
import WindowsMouse;

namespace gfl
{
	WindowsApp::WindowsApp(const AppConfiguration& appConfiguration, AppNotify* appNotify, Log* log) :
		appConfiguration{appConfiguration},
		appNotify{appNotify},
		log{log}
	{
		const auto hInstance{GetModuleHandle(nullptr)};
		const auto hIcon{LoadIcon(hInstance, appConfiguration.ResourceIconId ? MAKEINTRESOURCE(appConfiguration.ResourceIconId) : IDI_APPLICATION)};

		const WNDCLASSEX windowClassEx
		{
			.cbSize = sizeof(WNDCLASSEX),
			.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = WindowsApp::WindowProcedure,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = hInstance,
			.hIcon = hIcon,
			.hCursor = LoadCursor(nullptr, IDC_ARROW),
			.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
			.lpszMenuName = nullptr,
			.lpszClassName = WindowsApi::GetExecutableId().data(),
			.hIconSm = hIcon
		};

		if (!RegisterClassEx(&windowClassEx))
			this->log->Error("RegisterClassEx");

		const auto windowDimension{WindowsApi::GetWindowDimension(appConfiguration)};
		if (windowDimension.has_value())
		{
			const auto style{windowDimension.value().first};
			const auto& rect{windowDimension.value().second};
			this->handleWindow = CreateWindowEx(0, windowClassEx.lpszClassName, std::wstring{appConfiguration.Title.begin(), appConfiguration.Title.end()}.c_str(), style, rect.left, rect.top, rect.right, rect.bottom, nullptr, nullptr, hInstance, this);
			if (!this->handleWindow)
				this->log->Error("CreateWindowEx");
		}
		else
			this->log->Error("GetWindowDimension");
	}

	void WindowsApp::SetDisplayNotify(DisplayNotify* displayNotify)
	{
		this->displayNotify = displayNotify;
	}

	void WindowsApp::SetTitle(std::string_view title) const
	{
		SetWindowText(this->handleWindow, std::wstring{title.begin(), title.end()}.c_str());
	}

	void WindowsApp::ShowMessageBox(std::string_view title, std::string_view message) const
	{
		MessageBox(this->handleWindow, std::wstring{message.begin(), message.end()}.c_str(), std::wstring{title.begin(), title.end()}.c_str(), MB_OK | MB_SYSTEMMODAL);
	}

	void WindowsApp::Quit() const
	{
		PostQuitMessage(EXIT_SUCCESS);
	}

	int WindowsApp::Run()
	{
		ShowWindow(this->handleWindow, SW_SHOWNORMAL);

		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (this->appNotify)
					this->appNotify->OnRun();
			}
		}

		if (this->appNotify)
			this->appNotify->OnQuit();

		return static_cast<int>(msg.wParam);
	}

	LRESULT CALLBACK WindowsApp::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static constinit bool inSizeMove{};
		static constinit bool inSuspend{};
		static constinit bool minimized{};

		const auto app{reinterpret_cast<WindowsApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))};

		switch (message)
		{
		case WM_CREATE:
			if (lParam)
			{
				const auto params{reinterpret_cast<LPCREATESTRUCTW>(lParam)};
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(params->lpCreateParams));
			}
			break;

		case WM_PAINT:
			if (inSizeMove && app && app->appNotify)
				app->appNotify->OnRun();
			else
			{
				PAINTSTRUCT ps{};
				std::ignore = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;

		case WM_ACTIVATE:
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			WindowsMouse::ProcessMessage(message, wParam, lParam);
			break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			WindowsKeyboard::ProcessMessage(message, wParam, lParam);
			break;

		case WM_DISPLAYCHANGE:
			if (app && app->displayNotify)
				app->displayNotify->OnDisplayChange();
			break;

		case WM_MOVE:
			if (app && app->displayNotify)
				app->displayNotify->OnWindowMoved();
			break;

		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				if (!minimized)
				{
					minimized = true;
					if (!inSuspend && app && app->appNotify)
						app->appNotify->OnSuspending();
					inSuspend = true;
				}
			}
			else if (minimized)
			{
				minimized = false;
				if (inSuspend && app && app->appNotify)
					app->appNotify->OnResuming();
				inSuspend = false;
			}
			else if (!inSizeMove && app && app->displayNotify)
				app->displayNotify->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_ENTERSIZEMOVE:
			inSizeMove = true;
			break;

		case WM_EXITSIZEMOVE:
			inSizeMove = false;
			if (app && app->displayNotify)
			{
				RECT rc{};
				GetClientRect(hWnd, &rc);
				app->displayNotify->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
			}
			break;

		case WM_ACTIVATEAPP:
			if (app && app->appNotify)
			{
				WindowsKeyboard::ProcessMessage(message, wParam, lParam);
				WindowsMouse::ProcessMessage(message, wParam, lParam);

				if (wParam)
					app->appNotify->OnActivated();
				else
					app->appNotify->OnDeactivated();
			}
			break;

		case WM_POWERBROADCAST:
			switch (wParam)
			{
			case PBT_APMQUERYSUSPEND:
				if (!inSuspend && app && app->appNotify)
					app->appNotify->OnSuspending();
				inSuspend = true;
				return TRUE;

			case PBT_APMRESUMESUSPEND:
				if (!minimized)
				{
					if (inSuspend && app && app->appNotify)
						app->appNotify->OnResuming();
					inSuspend = false;
				}
				return TRUE;
			}
			break;

		case WM_SETCURSOR:
			if (app && !app->appConfiguration.ShowCursor)
			{
				static auto showCursor{app->appConfiguration.ShowCursor};
				const auto hitTest{LOWORD(lParam)};

				if (hitTest == HTCLIENT && !showCursor)
				{
					showCursor = true;
					ShowCursor(false);
				}
				else if (hitTest != HTCLIENT && showCursor)
				{
					showCursor = false;
					ShowCursor(true);
				}
			}
			break;

		case WM_MOUSEACTIVATE:
			return MA_ACTIVATEANDEAT; // When you click activate the window, we want Mouse to ignore that event.

		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE); // A menu is active and the user presses a key that does not correspond to any mnemonic or accelerator key. Ignore so we don't produce an error beep.

		case WM_SYSCOMMAND:
			if ((wParam == SC_SCREENSAVE) || (wParam == SC_MONITORPOWER) || (wParam == SC_KEYMENU))
				return 0;
			break;

		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}