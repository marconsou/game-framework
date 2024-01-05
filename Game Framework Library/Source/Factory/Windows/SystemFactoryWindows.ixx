export module SystemFactoryWindows;

import SystemFactory;

export namespace gfl
{
	class SystemFactoryWindows : public SystemFactory
	{
	public:
		std::unique_ptr<App> CreateApp(const AppConfiguration& appConfiguration, AppNotification* appNotification) override;
		std::unique_ptr<Video> CreateVideo(const VideoConfiguration& videoConfiguration, VideoSystem videoSystem, DeviceStateNotification* deviceStateNotification) override;
		std::unique_ptr<Input> CreateInput() override;
	private:
		AppConfiguration appConfiguration;
	};
}