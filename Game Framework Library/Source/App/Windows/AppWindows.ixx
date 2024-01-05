export module AppWindows;

import "WindowsCompact.h";
import App;
import AppConfiguration;
import AppNotification;

export namespace gfl
{
	class AppWindows : public App
	{
	public:
		AppWindows(const AppConfiguration& appConfiguration, AppNotification* appNotification);
		void SetTitle(std::string_view title) const override;
		void ShowMessageBox(std::string_view title, std::string_view message) const override;
		void Quit() const override;
		int OnRun(std::function<void()> onRun) override;
	private:
		HWND handleWindow{};
		AppNotification* appNotification{};
		AppConfiguration appConfiguration;
		std::function<void()> onRun;
		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}