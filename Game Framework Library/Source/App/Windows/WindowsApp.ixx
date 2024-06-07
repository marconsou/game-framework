module;

#include "WindowsCompact.h"

export module WindowsApp;

import App;
import AppConfiguration;
import AppNotify;
import DisplayNotify;
import Log;

export namespace gfl
{
	class WindowsApp : public App
	{
	public:
		WindowsApp(const AppConfiguration& appConfiguration, AppNotify* appNotify, Log* log);
		void SetDisplayNotify(DisplayNotify* displayNotify);
		void SetTitle(std::string_view title) const override;
		void ShowMessageBox(std::string_view title, std::string_view message) const override;
		void Quit() const override;
		int Run() override;
	private:
		AppConfiguration appConfiguration;
		AppNotify* appNotify{};
		DisplayNotify* displayNotify{};
		Log* log{};
		HWND handleWindow{};
		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}