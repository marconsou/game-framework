export module Game;

import <memory>;
import AppNotification;
import Color;
import DeviceStateNotification;
import StepTimer;
import Video;

import Input;


//
import App;
import EntryPoint;
import App;
import AppWindows;
import AppConfiguration;
import CrtDebugLogWindows;
import DateTime;
import Log;
import WindowsApi;
import Clock;
import Timer;
import ColorPalette;
import SystemFactory;
import SystemFactoryWindows;
import VideoConfiguration;
import VideoDirect3D12;
import VideoSystem;
import InputWindows;
//

export namespace gfl
{
	class Game : public AppNotification, public DeviceStateNotification
	{
	public:
		std::unique_ptr<App> App;
		std::unique_ptr<Video> Video;
		std::unique_ptr<Input> Input;
		StepTimer GlobalTimer;

		Game()
		{
			std::unique_ptr<SystemFactory> systemFactory = std::make_unique<SystemFactoryWindows>();
			const AppConfiguration appConfiguration{.Title = "Game App",.Width = 640,.Height = 480,.Windowed = true,.ShowCursor = true,.ResourceIconId = 0};
			const VideoConfiguration videoConfiguration{.VSync = false};
			this->App = systemFactory->CreateApp(appConfiguration, this);
			this->Video = systemFactory->CreateVideo(videoConfiguration, VideoSystem::Direct3D12, this);
			this->Input = systemFactory->CreateInput();

			this->CreateDeviceDependentResources();
			this->CreateWindowSizeDependentResources();

			// TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
			//   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
			//   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
			//   Add DX::DeviceResources::c_ReverseDepth to optimize depth buffer clears for 0 instead of 1.
		}

		void Run(const Color& clearColor)
		{
			this->GlobalTimer.Tick([&]() { this->Update(this->GlobalTimer); });

			if (this->GlobalTimer.GetFrameCount() == 0)
				return;

			this->Video->Render(clearColor);
		}

		void OnActivated() override
		{
			this->GlobalTimer.Resume();
		}

		void OnDeactivated() override
		{
			this->GlobalTimer.Pause();
		}

		void OnResuming() override
		{
			this->GlobalTimer.ResetElapsedTime();
		}

		void OnSuspending() override
		{

		}

		void OnQuit() override
		{

		}

		void OnWindowSizeChanged(int width, int height) override
		{
			if (!this->Video->OnWindowSizeChanged(width, height))
				return;

			this->CreateWindowSizeDependentResources();
		}

		void OnWindowMoved() override
		{
			this->Video->OnWindowMoved();
		}

		void OnDisplayChange() override
		{
			this->Video->OnDisplayChange();
		}

		void OnDeviceLost() override
		{

		}

		void OnDeviceRestored() override
		{
			this->CreateDeviceDependentResources();
			this->CreateWindowSizeDependentResources();
		}
	private:
		void CreateDeviceDependentResources()
		{

		}

		void CreateWindowSizeDependentResources()
		{

		}

		void Update(const StepTimer& timer)
		{

		}
	};
}