module VideoDirect3D12;

import "DirectX12.h";
import Log;
import WindowsApi;

namespace gfl
{
	VideoDirect3D12::VideoDirect3D12(const VideoConfiguration& videoConfiguration, DeviceStateNotification* deviceStateNotification) :
		videoConfiguration{videoConfiguration},
		deviceStateNotification{deviceStateNotification}
	{
		this->CheckForCPUFeatureSupport();
		this->CreateDeviceResources();
		this->CreateWindowSizeDependentResources();
	}

	VideoDirect3D12::~VideoDirect3D12()
	{
		this->WaitForGpu();
		this->ReportDebugLog();
	}

	void VideoDirect3D12::Render(const Color& clearColor)
	{
		this->Prepare();
		this->Clear(clearColor);
		this->Present();
	}

	bool VideoDirect3D12::OnWindowSizeChanged(int width, int height)
	{
		return this->WindowSizeChanged(width, height);
	}

	void VideoDirect3D12::OnWindowMoved()
	{
		this->WindowSizeChanged(this->videoConfiguration.Width, this->videoConfiguration.Height);
	}

	void VideoDirect3D12::OnDisplayChange()
	{
		this->UpdateColorSpace();
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE VideoDirect3D12::GetRenderTargetView() const
	{
		const auto cpuHandle = this->rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(cpuHandle, static_cast<INT>(this->backBufferIndex), this->rtvDescriptorSize);
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE VideoDirect3D12::GetDepthStencilView() const
	{
		const auto cpuHandle = this->dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(cpuHandle);
	}

	void VideoDirect3D12::CreateDeviceResources()
	{
		this->EnableDebugLog();
		this->CreateFactory();
		this->CheckForAllowTearingSupport();
		this->CreateDevice();
		this->ConfigureDebugLog();
		this->CheckForFeatureLevelSupport();
		this->CreateCommandQueue();
		this->CreateDescriptorHeap();
		this->CreateCommandAllocator();
		this->CreateCommandList();
		this->CreateFence();
		this->CheckForShaderModelSupport();
	}

	void VideoDirect3D12::CreateWindowSizeDependentResources()
	{
		this->WaitForGpu();
		this->CreateSwapChain();
		this->UpdateColorSpace();
		this->CreateRenderTargetView();
		this->ResetBackBufferIndex();
		this->CreateDepthStencilView();
		this->ViewportScissorSizeUpdate();
	}

	void VideoDirect3D12::CheckForCPUFeatureSupport()
	{
		if (!DirectX::XMVerifyCPUSupport())
			Log::Error("XMVerifyCPUSupport");
	}

	void VideoDirect3D12::EnableDebugLog()
	{
#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug> debug;
		if SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf())))
			debug->EnableDebugLayer();
		else
			Log::Warning("Direct3D Debug Device is not available!");

		Microsoft::WRL::ComPtr<IDXGIInfoQueue> infoQueue;
		if SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(infoQueue.GetAddressOf())))
		{
			this->factoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			if FAILED(infoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true))
				Log::Error("SetBreakOnSeverity DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR");

			if FAILED(infoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true))
				Log::Error("SetBreakOnSeverity DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION");

			DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
			{
				80, //IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides.
			};

			DXGI_INFO_QUEUE_FILTER filter
			{
				.DenyList =
				{
					.NumIDs = static_cast<UINT>(std::size(hide)),
					.pIDList = hide
				}
			};

			if FAILED(infoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter))
				Log::Error("AddStorageFilterEntries");
		}
		else
			Log::Error("DXGIGetDebugInterface");
#endif
	}

	void VideoDirect3D12::ReportDebugLog()
	{
#ifdef _DEBUG
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))
		{
			if FAILED(debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL)))
				Log::Error("ReportLiveObjects");
		}
		else
			Log::Error("DXGIGetDebugInterface");
#endif
	}

	void VideoDirect3D12::CreateFactory()
	{
		if FAILED(CreateDXGIFactory2(this->factoryFlags, IID_PPV_ARGS(this->factory.ReleaseAndGetAddressOf())))
			Log::Error("CreateDXGIFactory");
	}

	void VideoDirect3D12::CheckForAllowTearingSupport()
	{
		// Determines whether tearing support is available for fullscreen borderless windows.
		if (this->allowTearing)
		{
			auto enable = FALSE;

			Microsoft::WRL::ComPtr<IDXGIFactory5> factory;
			auto result = this->factory.As(&factory);
			if SUCCEEDED(result)
				result = factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &enable, sizeof(enable));

			if (FAILED(result) || !enable)
			{
				this->allowTearing = false;
				Log::Warning("Variable refresh rate displays not supported!");
			}
		}
	}

	void VideoDirect3D12::CreateDevice()
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		this->GetAdapter(adapter.GetAddressOf());

		if FAILED(D3D12CreateDevice(adapter.Get(), this->minFeatureLevel, IID_PPV_ARGS(this->device.ReleaseAndGetAddressOf())))
			Log::Error("D3D12CreateDevice");
	}

	void VideoDirect3D12::GetAdapter(IDXGIAdapter1** ppAdapter)
	{
		*ppAdapter = nullptr;

		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		Microsoft::WRL::ComPtr<IDXGIFactory6> factory;
		if SUCCEEDED(this->factory.As(&factory))
		{
			for (auto adapterIndex = 0; SUCCEEDED(factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))); ++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc{};
				if FAILED(adapter->GetDesc1(&desc))
					Log::Error("GetDesc");

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					continue;

				if SUCCEEDED(D3D12CreateDevice(adapter.Get(), this->minFeatureLevel, __uuidof(ID3D12Device), nullptr))
					break;
				else
					Log::Error("D3D12CreateDevice");
			}
		}
		else
			Log::Error("IDXGIFactory");

		if (!adapter)
		{
			for (auto adapterIndex = 0; SUCCEEDED(this->factory->EnumAdapters1(adapterIndex, adapter.ReleaseAndGetAddressOf())); ++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc{};
				if FAILED(adapter->GetDesc1(&desc))
					Log::Error("GetDesc");

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					continue;

				if SUCCEEDED(D3D12CreateDevice(adapter.Get(), this->minFeatureLevel, __uuidof(ID3D12Device), nullptr))
					break;
				else
					Log::Error("D3D12CreateDevice");
			}
		}

		if (!adapter)
			Log::Error("No Direct3D 12 device found!");

		*ppAdapter = adapter.Detach();
	}

	void VideoDirect3D12::ConfigureDebugLog()
	{
#ifndef NDEBUG
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
		if SUCCEEDED(this->device.As(&infoQueue))
		{
#ifdef _DEBUG
			if FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true))
				Log::Error("SetBreakOnSeverity D3D12_MESSAGE_SEVERITY_CORRUPTION");

			if FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true))
				Log::Error("SetBreakOnSeverity D3D12_MESSAGE_SEVERITY_ERROR");
#endif
			D3D12_MESSAGE_ID hide[] =
			{
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
				// Workarounds for debug layer issues on hybrid-graphics systems
				D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_WRONGSWAPCHAINBUFFERREFERENCE,
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE,
			};

			D3D12_INFO_QUEUE_FILTER filter
			{
				.DenyList =
				{
					.NumIDs = static_cast<UINT>(std::size(hide)),
					.pIDList = hide
				}
			};

			if FAILED(infoQueue->AddStorageFilterEntries(&filter))
				Log::Error("AddStorageFilterEntries");
		}
		else
			Log::Error("ID3D12InfoQueue");
#endif
	}

	void VideoDirect3D12::CheckForFeatureLevelSupport()
	{
		static constexpr D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelsData =
		{
			static_cast<UINT>(std::size(featureLevels)), featureLevels, D3D_FEATURE_LEVEL_11_0
		};

		if SUCCEEDED(this->device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelsData, sizeof(featureLevelsData)))
			this->featureLevel = featureLevelsData.MaxSupportedFeatureLevel;
		else
			this->featureLevel = this->minFeatureLevel;
	}

	void VideoDirect3D12::CreateCommandQueue()
	{
		const D3D12_COMMAND_QUEUE_DESC desc
		{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE
		};

		if FAILED(this->device->CreateCommandQueue(&desc, IID_PPV_ARGS(this->commandQueue.ReleaseAndGetAddressOf())))
			Log::Error("CreateCommandQueue");
	}

	void VideoDirect3D12::CreateDescriptorHeap()
	{
		const D3D12_DESCRIPTOR_HEAP_DESC rtvDesc
		{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			.NumDescriptors = this->backBufferCount
		};

		if FAILED(this->device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(this->rtvDescriptorHeap.ReleaseAndGetAddressOf())))
			Log::Error("CreateDescriptorHeap D3D12_DESCRIPTOR_HEAP_TYPE_RTV");

		this->rtvDescriptorSize = this->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		if (this->depthBufferFormat != DXGI_FORMAT_UNKNOWN)
		{
			const D3D12_DESCRIPTOR_HEAP_DESC dsvDesc
			{
				.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
				.NumDescriptors = 1
			};

			if FAILED(this->device->CreateDescriptorHeap(&dsvDesc, IID_PPV_ARGS(this->dsvDescriptorHeap.ReleaseAndGetAddressOf())))
				Log::Error("CreateDescriptorHeap D3D12_DESCRIPTOR_HEAP_TYPE_DSV");
		}
	}

	void VideoDirect3D12::CreateCommandAllocator()
	{
		this->commandAllocators.clear();
		this->commandAllocators.resize(this->backBufferCount);

		for (UINT i = 0; i < this->commandAllocators.size(); ++i)
		{
			if FAILED(this->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(this->commandAllocators[i].ReleaseAndGetAddressOf())))
				Log::Error("CreateCommandAllocator");
		}
	}

	void VideoDirect3D12::CreateCommandList()
	{
		if FAILED(this->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(this->commandList.ReleaseAndGetAddressOf())))
			Log::Error("CreateCommandList");

		if FAILED(this->commandList->Close())
			Log::Error("Close");
	}

	void VideoDirect3D12::CreateFence()
	{
		this->fenceValues.clear();
		this->fenceValues.resize(this->backBufferCount);

		if FAILED(this->device->CreateFence(this->fenceValues[this->backBufferIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(this->fence.ReleaseAndGetAddressOf())))
			Log::Error("CreateFence");

		this->fenceValues[this->backBufferIndex]++;

		this->fenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
		if (!this->fenceEvent.IsValid())
			Log::Error("Invalid Fence Event!");
	}

	void VideoDirect3D12::CheckForShaderModelSupport()
	{
		D3D12_FEATURE_DATA_SHADER_MODEL shaderModel{D3D_SHADER_MODEL_6_0};
		if (FAILED(this->device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel))) || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
			Log::Error("Shader Model is not supported!");
	}

	void VideoDirect3D12::WaitForGpu()
	{
		if (this->commandQueue && this->fence && this->fenceEvent.IsValid())
		{
			// Schedule a Signal command in the GPU queue.
			const auto fenceValue = this->fenceValues[this->backBufferIndex];
			if SUCCEEDED(this->commandQueue->Signal(this->fence.Get(), fenceValue))
			{
				// Wait until the Signal has been processed.
				if SUCCEEDED(this->fence->SetEventOnCompletion(fenceValue, this->fenceEvent.Get()))
				{
					std::ignore = WaitForSingleObjectEx(this->fenceEvent.Get(), INFINITE, FALSE);

					// Increment the fence value for the current frame.
					this->fenceValues[this->backBufferIndex]++;
				}
				else
					Log::Error("SetEventOnCompletion");
			}
			else
				Log::Error("Signal");
		}
		else
			Log::Error("WaitForGpu");
	}

	void VideoDirect3D12::MoveToNextFrame()
	{
		// Schedule a Signal command in the queue.
		const auto currentFenceValue = this->fenceValues[this->backBufferIndex];
		if FAILED(this->commandQueue->Signal(this->fence.Get(), currentFenceValue))
			Log::Error("Signal");

		// Update the back buffer index.
		this->ResetBackBufferIndex();

		// If the next frame is not ready to be rendered yet, wait until it is ready.
		if (this->fence->GetCompletedValue() < this->fenceValues[this->backBufferIndex])
		{
			if FAILED(this->fence->SetEventOnCompletion(this->fenceValues[this->backBufferIndex], this->fenceEvent.Get()))
				Log::Error("SetEventOnCompletion");

			std::ignore = WaitForSingleObjectEx(this->fenceEvent.Get(), INFINITE, FALSE);
		}

		// Set the fence value for the next frame.
		this->fenceValues[this->backBufferIndex] = currentFenceValue + 1;
	}

	void VideoDirect3D12::CreateSwapChain()
	{
		// Release resources that are tied to the swap chain and update fence values.
		for (auto& item : this->renderTargets)
			item.Reset();

		for (auto& item : this->fenceValues)
			item = this->fenceValues[this->backBufferIndex];

		const auto backBufferFormat = this->NoSRGB(this->backBufferFormat);

		if (this->swapChain)
		{
			const auto result = this->swapChain->ResizeBuffers(this->backBufferCount, this->videoConfiguration.Width, this->videoConfiguration.Height, backBufferFormat, this->allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u);
			if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
			{
				this->HandleDeviceLost();
				return;
			}
			else
				Log::Error("ResizeBuffers");
		}
		else
		{
			const DXGI_SWAP_CHAIN_DESC1 swapChainDesc
			{
				.Width = static_cast<UINT>(this->videoConfiguration.Width),
				.Height = static_cast<UINT>(this->videoConfiguration.Height),
				.Format = backBufferFormat,
				.SampleDesc =
				{
					.Count = 1,
					.Quality = 0
				},
				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
				.BufferCount = this->backBufferCount,
				.Scaling = DXGI_SCALING_STRETCH,
				.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
				.AlphaMode = DXGI_ALPHA_MODE_IGNORE,
				.Flags = this->allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u
			};

			const DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc
			{
				.Windowed = TRUE
			};

			const auto handleWindow = WindowsApi::GetHandleWindow();
			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
			if FAILED(this->factory->CreateSwapChainForHwnd(this->commandQueue.Get(), handleWindow, &swapChainDesc, &fsSwapChainDesc, nullptr, swapChain.GetAddressOf()))
				Log::Error("CreateSwapChainForHwnd");

			if FAILED(swapChain.As(&this->swapChain))
				Log::Error("IDXGISwapChain");

			if FAILED(this->factory->MakeWindowAssociation(handleWindow, DXGI_MWA_NO_ALT_ENTER))
				Log::Error("MakeWindowAssociation");
		}
	}

	void VideoDirect3D12::HandleDeviceLost()
	{
		if (this->deviceStateNotification)
			this->deviceStateNotification->OnDeviceLost();

		for (auto& item : this->commandAllocators)
			item.Reset();

		for (auto& item : this->renderTargets)
			item.Reset();

		this->depthStencil.Reset();
		this->commandQueue.Reset();
		this->commandList.Reset();
		this->fence.Reset();
		this->rtvDescriptorHeap.Reset();
		this->dsvDescriptorHeap.Reset();
		this->swapChain.Reset();
		this->device.Reset();
		this->factory.Reset();

		this->ReportDebugLog();

		this->CreateDeviceResources();
		this->CreateWindowSizeDependentResources();

		if (this->deviceStateNotification)
			this->deviceStateNotification->OnDeviceRestored();
	}

	void VideoDirect3D12::UpdateColorSpace()
	{
		if (!this->factory)
			return;

		if (!this->factory->IsCurrent())
		{
			// Output information is cached on the DXGI Factory. If it is stale we need to create a new factory.
			if FAILED(CreateDXGIFactory2(this->factoryFlags, IID_PPV_ARGS(this->factory.ReleaseAndGetAddressOf())))
				Log::Error("CreateDXGIFactory");
		}

		auto colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
		auto isDisplayHDR10 = false;

		// To detect HDR support, we will need to check the color space in the primary DXGI output associated with the app at this point in time (using window/display intersection).
		if (this->swapChain)
		{
			RECT windowBounds{};
			if (!GetWindowRect(WindowsApi::GetHandleWindow(), &windowBounds))
				Log::Error("GetWindowRect");

			const auto ax1 = windowBounds.left;
			const auto ay1 = windowBounds.top;
			const auto ax2 = windowBounds.right;
			const auto ay2 = windowBounds.bottom;

			auto bestIntersectArea = -1l;

			Microsoft::WRL::ComPtr<IDXGIOutput> bestOutput;
			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
			for (auto adapterIndex = 0; SUCCEEDED(this->factory->EnumAdapters(adapterIndex, adapter.ReleaseAndGetAddressOf())); ++adapterIndex)
			{
				Microsoft::WRL::ComPtr<IDXGIOutput> output;
				for (auto outputIndex = 0; SUCCEEDED(adapter->EnumOutputs(outputIndex, output.ReleaseAndGetAddressOf())); ++outputIndex)
				{
					DXGI_OUTPUT_DESC desc{};
					if FAILED(output->GetDesc(&desc))
						Log::Error("GetDesc");

					const auto intersectArea = this->ComputeIntersectionArea(ax1, ay1, ax2, ay2, desc.DesktopCoordinates.left, desc.DesktopCoordinates.top, desc.DesktopCoordinates.right, desc.DesktopCoordinates.bottom);
					if (intersectArea > bestIntersectArea)
					{
						bestOutput.Swap(output);
						bestIntersectArea = intersectArea;
					}
				}
			}

			if (bestOutput)
			{
				Microsoft::WRL::ComPtr<IDXGIOutput6> output;
				if SUCCEEDED(bestOutput.As(&output))
				{
					DXGI_OUTPUT_DESC1 desc{};
					if FAILED(output->GetDesc1(&desc))
						Log::Error("GetDesc");

					if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
						isDisplayHDR10 = true;
				}
				else
					Log::Error("IDXGIOutput");
			}
		}

		if (this->enableHDR && isDisplayHDR10)
		{
			switch (this->backBufferFormat)
			{
				// The application creates the HDR10 signal.
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
				break;

				// The system creates the HDR10 signal; application uses linear values.
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
				break;

			default:
				break;
			}
		}

		this->colorSpace = colorSpace;

		UINT colorSpaceSupport = 0;
		if (this->swapChain && SUCCEEDED(this->swapChain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport)) && (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
		{
			if FAILED(this->swapChain->SetColorSpace1(colorSpace))
				Log::Error("SetColorSpace");
		}
		else
			Log::Error("CheckColorSpaceSupport");
	}

	void VideoDirect3D12::CreateRenderTargetView()
	{
		this->renderTargets.clear();
		this->renderTargets.resize(this->backBufferCount);

		for (UINT i = 0; i < this->renderTargets.size(); ++i)
		{
			if FAILED(this->swapChain->GetBuffer(i, IID_PPV_ARGS(this->renderTargets[i].GetAddressOf())))
				Log::Error("GetBuffer");

			const D3D12_RENDER_TARGET_VIEW_DESC desc
			{
				.Format = this->backBufferFormat,
				.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
			};

			const auto cpuHandle = this->rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			const CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor{cpuHandle, static_cast<INT>(i), this->rtvDescriptorSize};
			this->device->CreateRenderTargetView(this->renderTargets[i].Get(), &desc, rtvDescriptor);
		}
	}

	void VideoDirect3D12::ResetBackBufferIndex()
	{
		this->backBufferIndex = this->swapChain->GetCurrentBackBufferIndex();
	}

	void VideoDirect3D12::CreateDepthStencilView()
	{
		if (this->depthBufferFormat != DXGI_FORMAT_UNKNOWN)
		{
			// Allocate a 2-D surface as the depth/stencil buffer and create a depth/stencil view on this surface.
			const CD3DX12_HEAP_PROPERTIES depthHeapProperties{D3D12_HEAP_TYPE_DEFAULT};

			D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(this->depthBufferFormat, this->videoConfiguration.Width, this->videoConfiguration.Height, 1, 1);
			depthStencilDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			const CD3DX12_CLEAR_VALUE depthOptimizedClearValue{this->depthBufferFormat, this->reverseDepth ? 0.0f : 1.0f, 0u};
			if FAILED(this->device->CreateCommittedResource(&depthHeapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthOptimizedClearValue, IID_PPV_ARGS(this->depthStencil.ReleaseAndGetAddressOf())))
				Log::Error("CreateCommittedResource");

			const D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc
			{
				.Format = this->depthBufferFormat,
				.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D
			};

			const auto cpuHandle = this->dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			this->device->CreateDepthStencilView(this->depthStencil.Get(), &dsvDesc, cpuHandle);
		}
	}

	void VideoDirect3D12::ViewportScissorSizeUpdate()
	{
		this->screenViewport.TopLeftX = this->screenViewport.TopLeftY = 0.0f;
		this->screenViewport.Width = static_cast<float>(this->videoConfiguration.Width);
		this->screenViewport.Height = static_cast<float>(this->videoConfiguration.Height);
		this->screenViewport.MinDepth = D3D12_MIN_DEPTH;
		this->screenViewport.MaxDepth = D3D12_MAX_DEPTH;

		this->scissorRect.left = this->scissorRect.top = 0;
		this->scissorRect.right = this->videoConfiguration.Width;
		this->scissorRect.bottom = this->videoConfiguration.Height;
	}

	bool VideoDirect3D12::WindowSizeChanged(int width, int height)
	{
		if (width == this->videoConfiguration.Width && height == this->videoConfiguration.Height)
		{
			this->UpdateColorSpace();
			return false;
		}

		this->videoConfiguration.Width = width;
		this->videoConfiguration.Height = height;
		this->CreateWindowSizeDependentResources();
		return true;
	}

	void VideoDirect3D12::Prepare(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
	{
		if FAILED(this->commandAllocators[this->backBufferIndex]->Reset())
			Log::Error("Reset");

		if FAILED(this->commandList->Reset(this->commandAllocators[this->backBufferIndex].Get(), nullptr))
			Log::Error("Reset");

		if (beforeState != afterState)
		{
			// Transition the render target into the correct state to allow for drawing into it.
			const D3D12_RESOURCE_BARRIER barrier{CD3DX12_RESOURCE_BARRIER::Transition(this->renderTargets[this->backBufferIndex].Get(), beforeState, afterState)};
			this->commandList->ResourceBarrier(1, &barrier);
		}
	}

	void VideoDirect3D12::Clear(const Color& clearColor)
	{
		const auto rtvDescriptor = this->GetRenderTargetView();
		const auto dsvDescriptor = this->GetDepthStencilView();

		this->commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
		this->commandList->ClearRenderTargetView(rtvDescriptor, &clearColor.GetFloat4()[0], 0, nullptr);
		this->commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		this->commandList->RSSetViewports(1, &this->screenViewport);
		this->commandList->RSSetScissorRects(1, &this->scissorRect);
	}

	void VideoDirect3D12::Present(D3D12_RESOURCE_STATES beforeState)
	{
		if (beforeState != D3D12_RESOURCE_STATE_PRESENT)
		{
			// Transition the render target to the state that allows it to be presented to the display.
			const D3D12_RESOURCE_BARRIER barrier{CD3DX12_RESOURCE_BARRIER::Transition(this->renderTargets[this->backBufferIndex].Get(), beforeState, D3D12_RESOURCE_STATE_PRESENT)};
			this->commandList->ResourceBarrier(1, &barrier);
		}

		// Send the command list off to the GPU for processing.
		if FAILED(this->commandList->Close())
			Log::Error("Close");

		this->commandQueue->ExecuteCommandLists(1, CommandListCast(this->commandList.GetAddressOf()));

		HRESULT result{};
		if (this->allowTearing)
		{
			// Recommended to always use tearing if supported when using a sync interval of 0. Note this will fail if in true 'fullscreen' mode.
			result = this->swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
		}
		else
		{
			// The first argument instructs DXGI to block until VSync, putting the application to sleep until the next VSync. This ensures we don't waste any cycles rendering frames that will never be displayed to the screen.
			result = this->swapChain->Present(1, 0);
		}

		// If the device was reset we must completely reinitialize the renderer.
		if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
		{
#ifdef _DEBUG
			Log::Warning("Device Lost: " + std::to_string(static_cast<unsigned int>((result == DXGI_ERROR_DEVICE_REMOVED) ? this->device->GetDeviceRemovedReason() : result)));
#endif
			this->HandleDeviceLost();
		}
		else
		{
			if FAILED(result)
				Log::Error("Present");

			this->MoveToNextFrame();

			if (!this->factory->IsCurrent())
				this->UpdateColorSpace();
		}
	}

	constexpr DXGI_FORMAT VideoDirect3D12::NoSRGB(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8X8_UNORM;
		default:                                return format;
		}
	}

	constexpr long VideoDirect3D12::ComputeIntersectionArea(long ax1, long ay1, long ax2, long ay2, long bx1, long by1, long bx2, long by2)
	{
		return std::max(0l, std::min(ax2, bx2) - std::max(ax1, bx1)) * std::max(0l, std::min(ay2, by2) - std::max(ay1, by1));
	}
}