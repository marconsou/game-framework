export module Game;

import "Windows.h";
import "DirectX12.h";
import AppNotification;
import DeviceNotification;
import StepTimer;
import VideoDirect3D12;
import VideoNotification;

export namespace gfl
{
	class Game : public DeviceNotification, public AppNotification, public VideoNotification
	{
	public:
		Game()
		{
			m_deviceResources = std::make_unique<VideoDirect3D12>();
			// TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
			//   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
			//   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
			//   Add DX::DeviceResources::c_ReverseDepth to optimize depth buffer clears for 0 instead of 1.
			m_deviceResources->RegisterDeviceNotify(this);
		}

		~Game()
		{
			if (m_deviceResources)
				m_deviceResources->WaitForGpu();
		}

		void Initialize(HWND window, int width, int height)
		{
			m_deviceResources->SetWindow(window, width, height);

			m_deviceResources->CreateDeviceResources();
			CreateDeviceDependentResources();

			m_deviceResources->CreateWindowSizeDependentResources();
			CreateWindowSizeDependentResources();

			// TODO: Change the timer settings if you want something other than the default variable timestep mode.
			// e.g. for 60 FPS fixed timestep update logic, call:

			m_timer.SetFixedTimeStep(true);
			m_timer.SetTargetElapsedSeconds(1.0 / 60);
		}

		void Tick()
		{
			m_timer.Tick([&]() { Update(m_timer); });
			Render();
		}

		// IDeviceNotify
		void OnDeviceLost() override
		{
			// TODO: Add Direct3D resource cleanup here.

			// If using the DirectX Tool Kit for DX12, uncomment this line:
			// m_graphicsMemory.reset();
		}

		void OnDeviceRestored() override
		{
			CreateDeviceDependentResources();
			CreateWindowSizeDependentResources();
		}

		void OnActivated() override
		{
			OutputDebugStringA("\nOnActivated\n");
		}

		void OnDeactivated() override
		{
			OutputDebugStringA("\nOnDeactivated\n");
		}

		void OnResuming() override
		{
			m_timer.ResetElapsedTime();
			OutputDebugStringA("\nOnResuming\n");
		}

		void OnSuspending() override
		{
			OutputDebugStringA("\nOnSuspending\n");
		}

		void OnClose() override
		{
			if (m_deviceResources)
			{
				m_deviceResources->WaitForGpu();
			}
			OutputDebugStringA("\nOnClose\n");
		}

		void OnWindowSizeChanged(int width, int height) override
		{
			if (!m_deviceResources->WindowSizeChanged(width, height))
				return;

			CreateWindowSizeDependentResources();

			// TODO: Game window is being resized.
		}

		void OnWindowMoved() override
		{
			auto const r = m_deviceResources->GetOutputSize();
			m_deviceResources->WindowSizeChanged(r.right, r.bottom);
		}

		void OnDisplayChange() override
		{
			m_deviceResources->UpdateColorSpace();
		}
	private:

		void Update(StepTimer const& timer)
		{
			//PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

			float elapsedTime = float(timer.GetElapsedSeconds());

			// TODO: Add your game logic here.
			elapsedTime;

			//PIXEndEvent();
		}

		void Render()
		{
			// Don't try to render anything before the first Update.
			if (m_timer.GetFrameCount() == 0)
			{
				return;
			}

			// Prepare the command list to render a new frame.
			m_deviceResources->Prepare();
			Clear();

			auto commandList = m_deviceResources->GetCommandList();
			//PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

			// TODO: Add your rendering code here.

			//PIXEndEvent(commandList);

			// Show the new frame.
			//PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
			m_deviceResources->Present();

			// If using the DirectX Tool Kit for DX12, uncomment this line:
			// m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());

			//PIXEndEvent();
		}

		void Clear()
		{
			auto commandList = m_deviceResources->GetCommandList();
			//PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

			// Clear the views.
			auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
			auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

			commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
			commandList->ClearRenderTargetView(rtvDescriptor, DirectX::Colors::CornflowerBlue, 0, nullptr);
			commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

			// Set the viewport and scissor rect.
			auto const viewport = m_deviceResources->GetScreenViewport();
			auto const scissorRect = m_deviceResources->GetScissorRect();
			commandList->RSSetViewports(1, &viewport);
			commandList->RSSetScissorRects(1, &scissorRect);

			//PIXEndEvent(commandList);
		}

		void CreateDeviceDependentResources()
		{
			auto device = m_deviceResources->GetD3DDevice();

			// Check Shader Model 6 support
			D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = {D3D_SHADER_MODEL_6_0};
			if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel))) || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
			{
#ifdef _DEBUG
				OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
				throw std::exception("Shader Model 6.0 is not supported!");
			}

			// If using the DirectX Tool Kit for DX12, uncomment this line:
			// m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

			// TODO: Initialize device dependent objects here (independent of window size).
		}

		void CreateWindowSizeDependentResources()
		{
			// TODO: Initialize windows-size dependent objects here.
		}

		// Device resources.
		//std::unique_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<VideoDirect3D12> m_deviceResources;

		// Rendering loop timer.
		StepTimer m_timer;

		// If using the DirectX Tool Kit for DX12, uncomment this line:
		// std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
	};
}