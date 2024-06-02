export module GameApp;

import AppConfiguration;
import ColorPalette;
import Game;
import VideoConfiguration;
import WindowsApi;
import WindowsSystemFactory;

import Timer;

using namespace gfl;

export class GameApp final : public Game
{
public:
	GameApp()
	{
		const AppConfiguration appConfiguration{.Title = "Game App",.Width = 800,.Height = 600,  .Windowed = true, .Resize = true,.ShowCursor = true};
		const VideoConfiguration videoConfiguration{.Width = appConfiguration.Width, .Height = appConfiguration.Height, .Windowed = appConfiguration.Windowed, .VSync = true};

		WindowsSystemFactory factory{this};

		this->log = factory.CreateFileLogger();
		this->app = factory.CreateApp(appConfiguration);
		this->video = factory.CreateVideo(videoConfiguration, VideoSystem::Direct3D11);
		this->input = factory.CreateInput();

		this->CreateDeviceDependentResources();
		this->CreateWindowSizeDependentResources();

		if (false)
		{
			this->timer.SetFixedTimeStep(true);
			this->timer.SetTargetElapsedSeconds(1.0 / 60);
		}

		//this->timer.Start();
	}
	Timer t;
	void Update()
	{
		float elapsedTime = float(this->timer.GetElapsedSeconds());

		this->app->SetTitle(std::format("{}fps {:.1f}s {}es {}et {}fc {}tt >> {}",
			this->timer.GetFramesPerSecond(),
			this->timer.GetTotalSeconds(),
			this->timer.GetElapsedSeconds(),
			this->timer.GetElapsedTicks(),
			this->timer.GetFrameCount(),
			this->timer.GetTotalTicks(), this->t.GetTime<std::chrono::seconds>()));
	}

	void OnRun()
	{
		this->input->Update();

		if (this->input->IsKeyboardKey(KeyboardKey::O, InputState::Pressed))
		{
			if (t.IsStarted())
				t.Stop();
			else
				t.Start();

			/*
			if (this->t.IsPaused())
				this->t.Resume();
			else
				this->t.Pause();
			*/
		}

		if (this->input->IsKeyboardKey(KeyboardKey::P, InputState::Pressed))
		{
			if (t.IsPaused())
				t.Resume();
			else
				t.Pause();
		}

		if (this->input->IsKeyboardKey(KeyboardKey::Escape, InputState::Released) || this->input->IsMouseButton(MouseButton::Right, InputState::Released))
			this->app->Quit();

		this->timer.Tick(this);

		if (this->timer.GetFrameCount() == 0)
			return;

		this->video->Render(ColorPalette::CornflowerBlue);
	}

	void OnActivated()
	{
		WindowsApi::OutputDebug("OnActivated");
		this->timer.SetFixedTimeStep(true);
		this->timer.SetTargetElapsedSeconds(1.0 / 120);

		if (!this->timer.IsStarted())
			this->timer.Start();
		else
			this->timer.Resume();
	}

	void OnDeactivated()
	{
		WindowsApi::OutputDebug("OnDeactivated");
		this->timer.SetFixedTimeStep(true);
		this->timer.SetTargetElapsedSeconds(1.0 / 15);

		this->timer.Pause();
	}

	void OnSuspending()
	{
		WindowsApi::OutputDebug("OnSuspending");
	}

	void OnResuming()
	{
		Game::OnResuming();
		WindowsApi::OutputDebug("OnResuming");
	}

	void OnQuit()
	{
		WindowsApi::OutputDebug("OnQuit");
	}

	void OnDeviceLost() override
	{

	}

	void OnResize(int width, int height) override
	{
		Game::OnResize(width, height);
		WindowsApi::OutputDebug("OnResize");
	}

	void CreateDeviceDependentResources() override
	{
		// These are the resources that depend on the device.
		// TODO: Initialize device dependent objects here (independent of window size).
	}

	void CreateWindowSizeDependentResources() override
	{
		// Allocate all memory resources that change on a window SizeChanged event.
		// TODO: Initialize windows-size dependent objects here.
	}
};