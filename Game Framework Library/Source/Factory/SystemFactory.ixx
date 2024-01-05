export module SystemFactory;

import <memory>;
import App;
import AppConfiguration;
import AppNotification;
import DeviceStateNotification;
import Input;
import Video;
import VideoConfiguration;
import VideoSystem;

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
		virtual std::unique_ptr<App> CreateApp(const AppConfiguration& appConfiguration, AppNotification* appNotification) = 0;
		virtual std::unique_ptr<Video> CreateVideo(const VideoConfiguration& videoConfiguration, VideoSystem videoSystem, DeviceStateNotification* deviceStateNotification) = 0;
		virtual std::unique_ptr<Input> CreateInput() = 0;
	};
}