//
// Game.cpp
//

//#include "Game.h"

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

using namespace DirectX;

using Microsoft::WRL::ComPtr;

namespace gfl
{
	Game::Game() : appConfiguration{.Title = "Direct3D 11 App",.Width = 800,.Height = 600,  .Windowed = true, .Resize = true,.ShowCursor = false}
	{
		this->fileLogger = std::make_unique<FileLogger>(this);
		//this->appConfiguration = std::make_unique({.Title = "Direct3D 11 App",.Width = 800,.Height = 600,  .Windowed = true, .Resize = false,.ShowCursor = true});
		this->windowsApp = std::make_unique<WindowsApp>(this->appConfiguration, this,this->fileLogger.get());

		m_deviceResources = std::make_unique<gfl::DeviceResources>();
		// TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
		//   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
		//   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
		m_deviceResources->RegisterDeviceNotify(this);

		this->windowsApp->SetDisplayNotify(this);


		this->Initialize(WindowsApi::GetHandleWindow(), appConfiguration.Width, appConfiguration.Height);
	}

	// Initialize the Direct3D resources required to run.
	void Game::Initialize(HWND window, int width, int height)
	{
		m_deviceResources->SetWindow(window, width, height);

		m_deviceResources->CreateDeviceResources();
		CreateDeviceDependentResources();

		m_deviceResources->CreateWindowSizeDependentResources();
		CreateWindowSizeDependentResources();

		// TODO: Change the timer settings if you want something other than the default variable timestep mode.
		// e.g. for 60 FPS fixed timestep update logic, call:
		/*
		m_timer.SetFixedTimeStep(true);
		m_timer.SetTargetElapsedSeconds(1.0 / 60);
		*/
	}

#pragma region Frame Update
	// Executes the basic game loop.
	void Game::Tick()
	{
		m_timer.Tick([&]()
			{
				Update(m_timer);
			});

		Render();
	}

	// Updates the world.
	void Game::Update(StepTimer const& timer)
	{
		float elapsedTime = float(timer.GetElapsedSeconds());

		// TODO: Add your game logic here.
		elapsedTime;
	}
#pragma endregion

#pragma region Frame Render
	// Draws the scene.
	void Game::Render()
	{
		// Don't try to render anything before the first Update.
		if (m_timer.GetFrameCount() == 0)
		{
			return;
		}

		Clear();

		m_deviceResources->PIXBeginEvent(L"Render");
		auto context = m_deviceResources->GetD3DDeviceContext();

		// TODO: Add your rendering code here.
		context;

		m_deviceResources->PIXEndEvent();

		// Show the new frame.
		m_deviceResources->Present();
	}

	// Helper method to clear the back buffers.
	void Game::Clear()
	{
		m_deviceResources->PIXBeginEvent(L"Clear");

		// Clear the views.
		auto context = m_deviceResources->GetD3DDeviceContext();
		auto renderTarget = m_deviceResources->GetRenderTargetView();
		auto depthStencil = m_deviceResources->GetDepthStencilView();

		context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
		context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		context->OMSetRenderTargets(1, &renderTarget, depthStencil);

		// Set the viewport.
		auto const viewport = m_deviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);

		m_deviceResources->PIXEndEvent();
	}
#pragma endregion

#pragma region Message Handlers

	void Game::OnRun()
	{
		this->Tick();

		this->windowsApp->SetTitle(std::format("{}fps {:.1f}s", this->m_timer.GetFramesPerSecond(), this->m_timer.GetTotalSeconds()));
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
		m_timer.ResetElapsedTime();

		OutputDebugString(L"\nOnResuming");
	}

	void Game::OnQuit()
	{
		OutputDebugString(L"\nOnQuit\n\n");
		
	}

	void Game::OnWindowMoved()
	{
		auto const r = m_deviceResources->GetOutputSize();
		m_deviceResources->WindowSizeChanged(r.right, r.bottom);

		OutputDebugString(L"\nOnWindowMoved");
	}

	void Game::OnDisplayChange()
	{
		m_deviceResources->UpdateColorSpace();

		OutputDebugString(L"\nOnDisplayChange");
	}

	void Game::OnWindowSizeChanged(int width, int height)
	{
		OutputDebugString(L"\nOnWindowSizeChanged [Begin]");

		if (!m_deviceResources->WindowSizeChanged(width, height))
			return;

		CreateWindowSizeDependentResources();

		// TODO: Game window is being resized.
		OutputDebugString(L"\nOnWindowSizeChanged [End]");
	}

#pragma endregion

#pragma region Direct3D Resources
	// These are the resources that depend on the device.
	void Game::CreateDeviceDependentResources()
	{
		auto device = m_deviceResources->GetD3DDevice();

		// TODO: Initialize device dependent objects here (independent of window size).
		device;
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
		CreateDeviceDependentResources();

		CreateWindowSizeDependentResources();
	}
#pragma endregion

}