export module Game;

import <memory>;
import App;
import AppNotification;
import Color;
import DeviceStateNotification;
import Input;
import StepTimer;
import Video;

export namespace gfl
{
	class Game : public AppNotification, public DeviceStateNotification
	{
	public:
		std::unique_ptr<App> App;
		std::unique_ptr<Video> Video;
		std::unique_ptr<Input> Input;
		StepTimer GlobalTimer;
		void Run(const Color& clearColor);
		void OnActivated() override;
		void OnDeactivated() override;
		void OnResuming() override;
		void OnSuspending() override;
		void OnQuit() override;
		void OnWindowSizeChanged(int width, int height) override;
		void OnWindowMoved() override;
		void OnDisplayChange() override;
		void OnDeviceLost() override;
		void OnDeviceRestored() override;
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void Update(const StepTimer& timer);
	};
}