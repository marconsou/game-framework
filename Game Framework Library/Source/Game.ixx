export module Game;

import App;
import AppNotify;
import DeviceNotify;
import Input;
import Log;
import LogNotify;
import StepTimer;
import Video;
import VideoNotify;

export namespace gfl
{
	class Game : public AppNotify, public DeviceNotify, public VideoNotify, public LogNotify
	{
	public:
		int Start();
	protected:
		std::unique_ptr<Log> log;
		std::unique_ptr<App> app;
		std::unique_ptr<Video> video;
		std::unique_ptr<Input> input;
		StepTimer timer;
		virtual void OnResuming() override;
		virtual void OnDeviceRestored() override;
		virtual void OnResize(int width, int height) override;
		virtual void OnLogError(std::string_view message) const override;
		virtual void CreateDeviceDependentResources() = 0;
		virtual void CreateWindowSizeDependentResources() = 0;
	};
}