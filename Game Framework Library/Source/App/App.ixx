export module App;

import <functional>;
import <string_view>;

export namespace gfl
{
	class App
	{
	public:
		App() = default;
		App(const App&) = delete;
		App(App&&) = delete;
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;
		virtual ~App() = default;
		virtual void SetTitle(std::string_view title) const = 0;
		virtual void ShowMessageBox(std::string_view title, std::string_view message) const = 0;
		virtual void Quit() const = 0;
		virtual int OnRun(std::function<void()> onRun) = 0;
	};
}