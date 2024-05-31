export module Game;

import AppConfiguration;
import AppNotify;
import Direct3D11Video;
import FileLogger;
import DeviceNotify;
import DeviceNotify;
import DisplayNotify;
import LogNotify;
import StepTimer;
import WindowsApp;

export namespace gfl
{
	class Game final : public DeviceNotify, public AppNotify, public DisplayNotify, public LogNotify
	{
	public:
		AppConfiguration appConfiguration;
		std::unique_ptr<FileLogger> fileLogger;
		std::unique_ptr<WindowsApp> windowsApp;
		std::unique_ptr<Direct3D11Video> video;
		StepTimer timer;

		Game();

		// DeviceNotify
		void OnDeviceLost() override;
		void OnDeviceRestored() override;

		// AppNotify
		void OnRun() override;
		void OnActivated() override;
		void OnDeactivated() override;
		void OnSuspending() override;
		void OnResuming() override;
		void OnQuit() override;

		// DisplayNotify
		void OnWindowMoved()override;
		void OnDisplayChange()override;
		void OnWindowSizeChanged(int width, int height)override;

		// LogNotify
		void OnLogError(std::string_view message) const override;
	private:
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
	};
}