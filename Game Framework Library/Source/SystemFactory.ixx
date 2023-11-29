export module SystemFactory;

import <memory>;
import App;
import AppConfiguration;
import AppNotification;
import VideoNotification;

export namespace gfl
{
	class SystemFactory
	{
	public:
		SystemFactory() = default;
		SystemFactory(const SystemFactory&) = delete;
		SystemFactory(SystemFactory&&) = delete;
		SystemFactory& operator=(const SystemFactory&) = delete;
		SystemFactory& operator=(SystemFactory&&) = delete;
		virtual ~SystemFactory() = default;
		virtual std::unique_ptr<App> CreateApp(const AppConfiguration& appConfiguration, AppNotification* appNotification = nullptr, VideoNotification* videoNotification = nullptr) = 0;
	};
}