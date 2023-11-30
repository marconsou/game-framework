export module SystemFactoryWindows;

import App;
import AppConfiguration;
import AppNotification;
import AppWindows;
import DeviceStateNotification;
import Input;
import InputWindows;
import SystemFactory;
import Video;
import VideoConfiguration;
import VideoDirect3D12;
import VideoSystem;

export namespace gfl
{
	class SystemFactoryWindows : public SystemFactory
	{
	public:
		std::unique_ptr<App> CreateApp(const AppConfiguration& appConfiguration, AppNotification* appNotification) override
		{
			this->appConfiguration = appConfiguration;
			return std::make_unique<AppWindows>(appConfiguration, appNotification);
		}

		std::unique_ptr<Video> CreateVideo(const VideoConfiguration& videoConfiguration, VideoSystem videoSystem, DeviceStateNotification* deviceStateNotification) override
		{
			VideoConfiguration configuration{videoConfiguration};
			configuration.Width = this->appConfiguration.Width;
			configuration.Height = this->appConfiguration.Height;
			configuration.Windowed = this->appConfiguration.Windowed;

			if (videoSystem == VideoSystem::Direct3D12)
				return std::make_unique<VideoDirect3D12>(configuration, deviceStateNotification);
			else if (videoSystem == VideoSystem::Vulkan)
			{

			}
			return nullptr;
		}

		std::unique_ptr<Input> CreateInput() override
		{
			return std::make_unique<InputWindows>();
		}
	private:
		AppConfiguration appConfiguration;
	};
}