export module SystemFactory;

import std;
import App;
import AppConfiguration;
import Input;
import Log;
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
		virtual std::unique_ptr<Log> CreateFileLogger() = 0;
		virtual std::unique_ptr<App> CreateApp(const AppConfiguration& appConfiguration) = 0;
		virtual std::unique_ptr<Video> CreateVideo(const VideoConfiguration& videoConfiguration, VideoSystem videoSystem) = 0;
		virtual std::unique_ptr<Input> CreateInput() = 0;
	};
}