export module WindowsSystemFactory;

import Direct3D11Video;
import GameNotify;
import SystemFactory;
import WindowsApp;

export namespace gfl
{
	class WindowsSystemFactory : public SystemFactory
	{
	public:
		WindowsSystemFactory(GameNotify* gameNotify);
		std::unique_ptr<Log> CreateFileLogger() override;
		std::unique_ptr<App> CreateApp(const AppConfiguration& appConfiguration) override;
		std::unique_ptr<Video> CreateVideo(const VideoConfiguration& videoConfiguration, VideoSystem videoSystem) override;
		std::unique_ptr<Input> CreateInput() override;
		std::unique_ptr<ShaderManager> CreateShaderManager() override;
	private:
		AppConfiguration appConfiguration;
		GameNotify* gameNotify{};
		Log* log{};
		WindowsApp* windowsApp{};
		Direct3D11Video* direct3D11Video{};
	};
}