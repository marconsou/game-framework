module WindowsSystemFactory;

import Direct3D11Video;
import FileLogger;
import GameNotify;
import WindowsInput;

namespace gfl
{
	WindowsSystemFactory::WindowsSystemFactory(GameNotify* gameNotify) : gameNotify{gameNotify}
	{

	}

	std::unique_ptr<Log> WindowsSystemFactory::CreateFileLogger()
	{
		auto log{std::make_unique<FileLogger>(this->gameNotify)};
		this->log = log.get();
		return log;
	}

	std::unique_ptr<App> WindowsSystemFactory::CreateApp(const AppConfiguration& appConfiguration)
	{
		auto windowsApp{std::make_unique<WindowsApp>(appConfiguration, this->gameNotify, this->log)};
		this->windowsApp = windowsApp.get();
		this->appConfiguration = appConfiguration;
		return windowsApp;
	}

	std::unique_ptr<Video> WindowsSystemFactory::CreateVideo(const VideoConfiguration& videoConfiguration, VideoSystem videoSystem)
	{
		VideoConfiguration configuration{videoConfiguration};
		configuration.Width = this->appConfiguration.Width;
		configuration.Height = this->appConfiguration.Height;
		configuration.Windowed = this->appConfiguration.Windowed;

		if (videoSystem == VideoSystem::Direct3D11)
		{
			auto direct3D11Video{std::make_unique<Direct3D11Video>(configuration, this->gameNotify, this->log)};
			this->windowsApp->SetDisplayNotify(direct3D11Video->GetDisplayNotify());
			direct3D11Video->SetVideoNotify(this->gameNotify);
			return direct3D11Video;
		}
		return nullptr;
	}

	std::unique_ptr<Input> WindowsSystemFactory::CreateInput()
	{
		return std::make_unique<WindowsInput>();
	}
}