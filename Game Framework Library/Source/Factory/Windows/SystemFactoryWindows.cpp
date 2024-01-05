module SystemFactoryWindows;

import AppWindows;
import InputWindows;
import VideoDirect3D12;

namespace gfl
{
	std::unique_ptr<App> SystemFactoryWindows::CreateApp(const AppConfiguration& appConfiguration, AppNotification* appNotification)
	{
		this->appConfiguration = appConfiguration;
		return std::make_unique<AppWindows>(appConfiguration, appNotification);
	}

	std::unique_ptr<Video> SystemFactoryWindows::CreateVideo(const VideoConfiguration& videoConfiguration, VideoSystem videoSystem, DeviceStateNotification* deviceStateNotification)
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

	std::unique_ptr<Input> SystemFactoryWindows::CreateInput()
	{
		return std::make_unique<InputWindows>();
	}
}