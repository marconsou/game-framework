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

		void Run(const Color& clearColor)
		{
			this->GlobalTimer.Tick([&]() { this->Update(this->GlobalTimer); });

			if (this->GlobalTimer.GetFrameCount() == 0)
				return;

			if (this->Video)
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
			if (this->Video && !this->Video->OnWindowSizeChanged(width, height))
				return;

			this->CreateWindowSizeDependentResources();
		}

		void OnWindowMoved() override
		{
			if (this->Video)
				this->Video->OnWindowMoved();
		}

		void OnDisplayChange() override
		{
			if (this->Video)
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