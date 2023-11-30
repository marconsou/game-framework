export module AppWindows;

import <functional>;
import <string_view>;
import "Windows.h";
import App;
import AppConfiguration;
import AppNotification;
import KeyboardWindows;
import Log;
import MouseWindows;
import WindowsApi;

export namespace gfl
{
	class AppWindows : public App
	{
	public:
		AppWindows(const AppConfiguration& appConfiguration, AppNotification* appNotification) :
			configuration{appConfiguration},
			appNotification{appNotification}
		{
			const auto hInstance = GetModuleHandle(nullptr);
			const auto hIcon = LoadIcon(hInstance, appConfiguration.ResourceIconId ? MAKEINTRESOURCE(appConfiguration.ResourceIconId) : IDI_APPLICATION);

			WNDCLASSEX windowClassEx
			{
				.cbSize = sizeof(WNDCLASSEX),
				.style = CS_HREDRAW | CS_VREDRAW,
				.lpfnWndProc = AppWindows::WindowProcedure,
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
				Log::Error("RegisterClassEx");

			const auto windowDimension = WindowsApi::GetWindowDimension(appConfiguration);
			this->handleWindow = CreateWindowEx(0, windowClassEx.lpszClassName, std::wstring{appConfiguration.Title.begin(), appConfiguration.Title.end()}.c_str(), windowDimension.first, windowDimension.second.left, windowDimension.second.top, windowDimension.second.right, windowDimension.second.bottom, nullptr, nullptr, hInstance, this);
			if (!this->handleWindow)
				Log::Error("CreateWindowEx");
		}

		void SetTitle(std::string_view title) const override
		{
			SetWindowText(this->handleWindow, std::wstring{title.begin(), title.end()}.c_str());
		}

		void ShowMessageBox(std::string_view title, std::string_view message) const override
		{
			MessageBox(this->handleWindow, std::wstring{message.begin(), message.end()}.c_str(), std::wstring{title.begin(), title.end()}.c_str(), MB_OK | MB_SYSTEMMODAL);
		}

		void Quit() const override
		{
			PostQuitMessage(0);
		}

		int OnRun(std::function<void()> onRun) override
		{
			this->onRun = onRun;
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
					if (this->onRun)
						this->onRun();
				}
			}

			if (this->appNotification)
				this->appNotification->OnQuit();

			return static_cast<int>(msg.wParam);
		}
	private:
		HWND handleWindow{};
		AppNotification* appNotification{};
		AppConfiguration configuration;
		std::function<void()> onRun;

		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			static constinit bool inSizeMove = false;
			static constinit bool inSuspend = false;
			static constinit bool minimized = false;

			const auto app = reinterpret_cast<AppWindows*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			switch (message)
			{
			case WM_CREATE:
				if (lParam)
				{
					const auto params = reinterpret_cast<LPCREATESTRUCTW>(lParam);
					SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(params->lpCreateParams));
				}
				break;

			case WM_PAINT:
				if (inSizeMove && app && app->onRun)
					app->onRun();
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
				MouseWindows::ProcessMessage(message, wParam, lParam);
				break;

			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
				KeyboardWindows::ProcessMessage(message, wParam, lParam);
				break;

			case WM_DISPLAYCHANGE:
				if (app && app->appNotification)
					app->appNotification->OnDisplayChange();
				break;

			case WM_MOVE:
				if (app && app->appNotification)
					app->appNotification->OnWindowMoved();
				break;

			case WM_SIZE:
				if (wParam == SIZE_MINIMIZED)
				{
					if (!minimized)
					{
						minimized = true;
						if (!inSuspend && app && app->appNotification)
							app->appNotification->OnSuspending();
						inSuspend = true;
					}
				}
				else if (minimized)
				{
					minimized = false;
					if (inSuspend && app && app->appNotification)
						app->appNotification->OnResuming();
					inSuspend = false;
				}
				else if (!inSizeMove && app && app->appNotification)
					app->appNotification->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
				break;

			case WM_ENTERSIZEMOVE:
				inSizeMove = true;
				break;

			case WM_EXITSIZEMOVE:
				inSizeMove = false;
				if (app && app->appNotification)
				{
					RECT rc{};
					GetClientRect(hWnd, &rc);
					app->appNotification->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
				}
				break;

			case WM_ACTIVATEAPP:
				if (app && app->appNotification)
				{
					if (wParam)
						app->appNotification->OnActivated();
					else
						app->appNotification->OnDeactivated();
				}
				break;

			case WM_POWERBROADCAST:
				switch (wParam)
				{
				case PBT_APMQUERYSUSPEND:
					if (!inSuspend && app && app->appNotification)
						app->appNotification->OnSuspending();
					inSuspend = true;
					return TRUE;

				case PBT_APMRESUMESUSPEND:
					if (!minimized)
					{
						if (inSuspend && app && app->appNotification)
							app->appNotification->OnResuming();
						inSuspend = false;
					}
					return TRUE;
				}
				break;

			case WM_SETCURSOR:
				if (app && !app->configuration.ShowCursor)
				{
					static auto showCursor = app->configuration.ShowCursor;
					const auto hitTest = LOWORD(lParam);

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
				if (!DestroyWindow(hWnd))
					Log::Error("DestroyWindow");
				break;

			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	};
}