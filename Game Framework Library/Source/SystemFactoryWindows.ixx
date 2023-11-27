export module SystemFactoryWindows;

import <memory>;
import App;
import AppConfiguration;
import AppNotification;
import AppWindows;
import SystemFactory;

export namespace gfl
{
	class SystemFactoryWindows : public SystemFactory
	{
	public:
		std::unique_ptr<App> CreateApp(const AppConfiguration& appConfiguration, AppNotification* appNotification) override
		{
			auto app = std::make_unique<AppWindows>(appConfiguration, appNotification);
			this->appWindows = app.get();
			this->appConfiguration = appConfiguration;
			return app;
		}
	private:
		AppWindows* appWindows{};
		AppConfiguration appConfiguration;
	};
}