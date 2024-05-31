module;

#include "../Game Framework Library/Source/Video/Render/Direct3D11/Direct3D11.h"

module Game;

//import "D:/Downloads/Game Framework/Game Framework Library/Source/Video/Render/Direct3D11/Direct3D11.h";

import std;
import Clock;
import DateTime;
import EntryPoint;
import WindowsApi;
import WindowsApp;
import AppConfiguration;
import FileLogger;
import VideoConfiguration;
import ColorPalette;


/*

class Direct3DVideo
	{
	public:
		Direct3DVideo() = default;
		Direct3DVideo(const Direct3DVideo&) = delete;
		Direct3DVideo(Direct3DVideo&&) = delete;
		Direct3DVideo& operator=(const Direct3DVideo&) = delete;
		Direct3DVideo& operator=(Direct3DVideo&&) = delete;
		virtual ~Direct3DVideo() = default;
		virtual void OnWindowSizeChanged(int width, int height) = 0;
		virtual void OnWindowMoved() = 0;
		virtual void OnDisplayChange() = 0;
	};

 */


class Direct3DDeviceResource
{

};

namespace gfl
{
	Game::Game() : appConfiguration{.Title = "Direct3D 11 App",.Width = 800,.Height = 600,  .Windowed = true, .Resize = true,.ShowCursor = true}
	{
		this->fileLogger = std::make_unique<FileLogger>(this);
		this->windowsApp = std::make_unique<WindowsApp>(this->appConfiguration, this, this->fileLogger.get());

		VideoConfiguration videoConfiguration{.Width = this->appConfiguration.Width, .Height = this->appConfiguration.Height, .Windowed = this->appConfiguration.Windowed, .VSync = true};
		this->video = std::make_unique<gfl::Direct3D11Video>(videoConfiguration, this, this->fileLogger.get());
		// TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
		//   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
		//   Add DX::DeviceResources::c_EnableHDR for HDR10 display.

		this->windowsApp->SetDisplayNotify(this);


		//m_deviceResources->SetWindow(window, width, height);

		//m_deviceResources->CreateDeviceResources();
		this->CreateDeviceDependentResources();

		//m_deviceResources->CreateWindowSizeDependentResources();
		this->CreateWindowSizeDependentResources();

		// TODO: Change the timer settings if you want something other than the default variable timestep mode.
		// e.g. for 60 FPS fixed timestep update logic, call:
		/*
		m_timer.SetFixedTimeStep(true);
		m_timer.SetTargetElapsedSeconds(1.0 / 60);
		*/
	}

#pragma region Message Handlers

	void Game::OnRun()
	{
		this->timer.Tick([&]()
		{
			float elapsedTime = float(this->timer.GetElapsedSeconds());
		});

		// Don't try to render anything before the first Update.
		if (this->timer.GetFrameCount() == 0)
			return;

		this->video->Render(ColorPalette::CornflowerBlue);

		this->windowsApp->SetTitle(std::format("{}fps {:.1f}s", this->timer.GetFramesPerSecond(), this->timer.GetTotalSeconds()));
	}

	void Game::OnActivated()
	{
		OutputDebugString(L"\nOnActivated");
	}

	void Game::OnDeactivated()
	{
		OutputDebugString(L"\nOnDeactivated");
	}

	void Game::OnSuspending()
	{
		OutputDebugString(L"\nOnSuspending");
	}

	void Game::OnResuming()
	{
		this->timer.ResetElapsedTime();
		OutputDebugString(L"\nOnResuming");
	}

	void Game::OnQuit()
	{
		OutputDebugString(L"\nOnQuit\n\n");
	}

	void Game::OnWindowMoved()
	{
		this->video->WindowSizeChanged();

		OutputDebugString(L"\nOnWindowMoved");
	}

	void Game::OnDisplayChange()
	{
		this->video->UpdateColorSpace();

		OutputDebugString(L"\nOnDisplayChange");
	}

	void Game::OnWindowSizeChanged(int width, int height)
	{
		OutputDebugString(L"\nOnWindowSizeChanged [Begin]");

		if (!this->video->WindowSizeChanged(width, height))
			return;

		this->CreateWindowSizeDependentResources();

		// TODO: Game window is being resized.
		OutputDebugString(L"\nOnWindowSizeChanged [End]");
	}

#pragma endregion

#pragma region Direct3D Resources
	// These are the resources that depend on the device.
	void Game::CreateDeviceDependentResources()
	{
		// TODO: Initialize device dependent objects here (independent of window size).
	}

	// Allocate all memory resources that change on a window SizeChanged event.
	void Game::CreateWindowSizeDependentResources()
	{
		// TODO: Initialize windows-size dependent objects here.
	}

	void Game::OnDeviceLost()
	{
		// TODO: Add Direct3D resource cleanup here.
	}

	void Game::OnDeviceRestored()
	{
		this->CreateDeviceDependentResources();
		this->CreateWindowSizeDependentResources();
	}
#pragma endregion

	void Game::OnLogError(std::string_view message) const
	{
		throw std::exception(message.data());
	}
}