module;

#include "Direct3D11.h"

module Direct3D11Video;

import File;
import WindowsApi;

namespace gfl
{
	Direct3D11Video::Direct3D11Video(const VideoConfiguration& videoConfiguration, DeviceNotify* deviceNotify, Log* log) :
		videoConfiguration{videoConfiguration},
		deviceNotify{deviceNotify},
		log{log},
		direct3DDisplayEvent{this}
	{
		this->CheckForCPUFeatureSupport();
		this->CreateDeviceResources();
		this->CreateWindowSizeDependentResources();
	}

	Direct3D11Video::~Direct3D11Video()
	{
		this->Cleanup();
	}

	void Direct3D11Video::ResetDeviceResources()
	{
		this->shaders.Clear();
		this->vertexBuffers.Clear();
	}

	void Direct3D11Video::ResetWindowSizeDependentResources()
	{
		this->renderTargetView.Reset();
		this->depthStencilView.Reset();
		this->renderTarget.Reset();
		this->depthStencil.Reset();
	}

	void Direct3D11Video::Cleanup()
	{
		this->ResetDeviceResources();
		this->ResetWindowSizeDependentResources();

		this->swapChain.Reset();

		this->context->ClearState();
		this->context->Flush();
		this->context.Reset();

		this->ReportDebug();

		this->device.Reset();
		this->dxgiFactory.Reset();
	}

	void Direct3D11Video::SetVideoNotify(VideoNotify* videoNotify)
	{
		this->direct3DDisplayEvent.SetVideoNotify(videoNotify);
	}

	DisplayNotify* Direct3D11Video::GetDisplayNotify()
	{
		return &this->direct3DDisplayEvent;
	}

	ID3D11Device1* Direct3D11Video::GetDevice() const
	{
		return this->device.Get();
	}

	ID3D11DeviceContext1* Direct3D11Video::GetContext() const
	{
		return this->context.Get();
	}

	void Direct3D11Video::AddShader(std::string_view resourceName, std::unique_ptr<Direct3D11Shader> direct3D11Shader)
	{
		this->shaders.Add(resourceName.data(), std::move(direct3D11Shader));
	}

	const Direct3D11Shader* Direct3D11Video::GetShader(std::string_view resourceName) const
	{
		return this->shaders.Get(resourceName);
	}

	void Direct3D11Video::CheckForCPUFeatureSupport()
	{
		if (!DirectX::XMVerifyCPUSupport())
			this->log->Error("XMVerifyCPUSupport");
	}

	void Direct3D11Video::CreateDeviceResources()
	{
		this->ResetDeviceResources();

		const auto creationFlags{this->CheckForSDKLayerSupport()};
		this->CreateFactory();
		this->CheckForFeaturesSupport();
		this->CreateDevice(creationFlags);
		this->CreateDebug();
		this->CreateBuffers();
	}

	void Direct3D11Video::CreateWindowSizeDependentResources()
	{
		const auto handleWindow{WindowsApi::GetHandleWindow()};
		if (!handleWindow)
			this->log->Error("Invalid window handle!");

		this->context->ClearState();
		this->ResetWindowSizeDependentResources();
		this->context->Flush();

		const auto backBufferWidth{static_cast<UINT>(this->videoConfiguration.Width)};
		const auto backBufferHeight{static_cast<UINT>(this->videoConfiguration.Height)};
		const auto backBufferFormat{(this->flipPresent || this->allowTearing || this->enableHDR) ? Direct3D11Video::NoSRGB(this->backBufferFormat) : this->backBufferFormat};

		if (this->swapChain)
		{
			const auto result{this->swapChain->ResizeBuffers(this->backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, this->allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u)};
			if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
			{
				const auto code{static_cast<unsigned int>((result == DXGI_ERROR_DEVICE_REMOVED) ? this->device->GetDeviceRemovedReason() : result)};
				this->log->Warning(std::format("Device Lost on ResizeBuffers: {}", code));
				this->HandleDeviceLost();
				return;
			}
			else
			{
				if (WindowsApi::Failed(result))
					this->log->Error("ResizeBuffers");
			}
		}
		else
		{
			const DXGI_SWAP_CHAIN_DESC1 swapChainDesc
			{
				.Width = backBufferWidth,
				.Height = backBufferHeight,
				.Format = backBufferFormat,
				.Stereo = FALSE,
				.SampleDesc =
				{
					.Count = 1,
					.Quality = 0,
				},
				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
				.BufferCount = this->backBufferCount,
				.Scaling = DXGI_SCALING_STRETCH,
				.SwapEffect = (this->flipPresent || this->allowTearing || this->enableHDR) ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD,
				.AlphaMode = DXGI_ALPHA_MODE_IGNORE,
				.Flags = (this->allowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u
			};

			const DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenSwapChainDesc
			{
				.RefreshRate =
				{
					.Numerator = 0,
					.Denominator = 0
				},
				.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
				.Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
				.Windowed = TRUE
			};

			if (WindowsApi::Failed(this->dxgiFactory->CreateSwapChainForHwnd(this->device.Get(), handleWindow, &swapChainDesc, &fullScreenSwapChainDesc, nullptr, this->swapChain.ReleaseAndGetAddressOf())))
				this->log->Error("CreateSwapChainForHwnd");

			if (WindowsApi::Failed(this->dxgiFactory->MakeWindowAssociation(handleWindow, DXGI_MWA_NO_ALT_ENTER)))
				this->log->Error("MakeWindowAssociation");
		}

		this->UpdateColorSpace();

		if (WindowsApi::Failed(this->swapChain->GetBuffer(0, IID_PPV_ARGS(this->renderTarget.ReleaseAndGetAddressOf()))))
			this->log->Error("GetBuffer");

		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{D3D11_RTV_DIMENSION_TEXTURE2D, this->backBufferFormat};
		if (WindowsApi::Failed(this->device->CreateRenderTargetView(this->renderTarget.Get(), &renderTargetViewDesc, this->renderTargetView.ReleaseAndGetAddressOf())))
			this->log->Error("CreateRenderTargetView");

		if (this->depthBufferFormat != DXGI_FORMAT_UNKNOWN)
		{
			CD3D11_TEXTURE2D_DESC depthStencilDesc{this->depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL};
			if (WindowsApi::Failed(this->device->CreateTexture2D(&depthStencilDesc, nullptr, this->depthStencil.ReleaseAndGetAddressOf())))
				this->log->Error("CreateTexture2D");

			if (WindowsApi::Failed(this->device->CreateDepthStencilView(this->depthStencil.Get(), nullptr, this->depthStencilView.ReleaseAndGetAddressOf())))
				this->log->Error("CreateDepthStencilView");
		}

		this->screenViewport = {0.0f, 0.0f, static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight), 0.0f, 1.0f};
	}

	void Direct3D11Video::CreateBuffers()
	{
		this->vertexBuffers.Add("Dynamic", std::make_unique<Direct3D11VertexBuffer>(1024, this->device.Get(), this->log));
	}

	UINT Direct3D11Video::CheckForSDKLayerSupport()
	{
		UINT creationFlags{D3D11_CREATE_DEVICE_BGRA_SUPPORT};
#ifdef _DEBUG
		if (WindowsApi::Succeeded(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_NULL, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, nullptr, nullptr, nullptr)))
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
		else
			this->log->Warning("Direct3D Debug Device is not available!");
#endif
		return creationFlags;
	}

	void Direct3D11Video::CreateFactory()
	{
#ifdef _DEBUG 
		bool debugDXGI{};
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (WindowsApi::Succeeded(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			debugDXGI = true;

			if (WindowsApi::Failed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(this->dxgiFactory.ReleaseAndGetAddressOf()))))
				this->log->Error("CreateDXGIFactory");

			if (WindowsApi::Failed(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true)))
				this->log->Error("SetBreakOnSeverity - DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR");

			if (WindowsApi::Failed(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true)))
				this->log->Error("SetBreakOnSeverity - DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION");

			DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
			{
				80, //IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides
			};

			DXGI_INFO_QUEUE_FILTER filter
			{
				.DenyList
				{
					.NumIDs = static_cast<UINT>(std::size(hide)),
					.pIDList = hide
				}
			};

			if (WindowsApi::Failed(dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter)))
				this->log->Error("AddStorageFilterEntries");
		}
		else
			this->log->Error("DXGIGetDebugInterface");

		if (!debugDXGI)
#endif
			if (WindowsApi::Failed(CreateDXGIFactory1(IID_PPV_ARGS(this->dxgiFactory.ReleaseAndGetAddressOf()))))
				this->log->Error("CreateDXGIFactory");
	}

	void Direct3D11Video::CheckForFeaturesSupport()
	{
		// Determines whether tearing support is available for fullscreen borderless windows
		if (this->allowTearing)
		{
			auto allowTearing{FALSE};
			Microsoft::WRL::ComPtr<IDXGIFactory5> factory5;
			auto result{this->dxgiFactory.As(&factory5)};

			if (WindowsApi::Succeeded(result))
				result = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));

			if (WindowsApi::Failed(result) || !allowTearing)
			{
				this->allowTearing = false;
				this->log->Warning("Variable refresh rate displays not supported!");
			}
		}

		// Disable HDR if we are on an OS that can't support FLIP swap effects
		if (this->enableHDR)
		{
			Microsoft::WRL::ComPtr<IDXGIFactory5> factory5;
			if (WindowsApi::Failed(this->dxgiFactory.As(&factory5)))
			{
				this->enableHDR = false;
				this->log->Warning("HDR swap chains not supported!");
			}
		}

		// Disable FLIP if not on a supporting OS
		if (this->flipPresent)
		{
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
			if (WindowsApi::Failed(this->dxgiFactory.As(&factory4)))
			{
				this->flipPresent = false;
				this->log->Warning("Flip swap effects not supported!");
			}
		}
	}

	void Direct3D11Video::CreateDevice(UINT creationFlags)
	{
		constexpr D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_12_1,
		};

		int featureLevelCount{};
		for (; featureLevelCount < static_cast<UINT>(std::size(featureLevels)); ++featureLevelCount)
		{
			if (featureLevels[featureLevelCount] < this->featureLevel)
				break;
		}

		if (!featureLevelCount)
			this->log->Error("Direct3D minimum feature level too high!");

		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

		this->GetHardwareAdapter(adapter.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

		auto result{E_FAIL};
		if (adapter)
			result = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, device.GetAddressOf(), &this->featureLevel, context.GetAddressOf());
#ifdef NDEBUG
		else
			this->log->Error("No Direct3D hardware device found!");
#else
		if (WindowsApi::Failed(result))
			result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, creationFlags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, device.GetAddressOf(), &this->featureLevel, context.GetAddressOf());
#endif

		if (WindowsApi::Failed(result))
			this->log->Error("D3D11CreateDevice");

		if (WindowsApi::Failed(device.As(&this->device)))
			this->log->Error("ID3D11Device");

		if (WindowsApi::Failed(context.As(&this->context)))
			this->log->Error("ID3D11DeviceContext");
	}

	void Direct3D11Video::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
	{
		*ppAdapter = nullptr;

		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;
		if (WindowsApi::Succeeded(this->dxgiFactory.As(&factory6)))
		{
			for (auto adapterIndex{0}; WindowsApi::Succeeded(factory6->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))); ++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc{};
				if (WindowsApi::Failed(adapter->GetDesc1(&desc)))
					this->log->Error("GetDesc");

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					continue;

				break;
			}
		}
		else
			this->log->Error("IDXGIFactory");

		if (!adapter)
		{
			for (auto adapterIndex{0}; WindowsApi::Succeeded(this->dxgiFactory->EnumAdapters1(adapterIndex, adapter.ReleaseAndGetAddressOf())); ++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc{};
				if (WindowsApi::Failed(adapter->GetDesc1(&desc)))
					this->log->Error("GetDesc");

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					continue;

				break;
			}
		}

		*ppAdapter = adapter.Detach();
	}

	void Direct3D11Video::CreateDebug()
	{
#ifndef NDEBUG
		Microsoft::WRL::ComPtr<ID3D11Debug> debug;
		if (WindowsApi::Succeeded(this->device.As(&debug)))
		{
			Microsoft::WRL::ComPtr<ID3D11InfoQueue> infoQueue;
			if (WindowsApi::Succeeded(debug.As(&infoQueue)))
			{
#ifdef _DEBUG
				if (WindowsApi::Failed(infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true)))
					this->log->Error("SetBreakOnSeverity - D3D11_MESSAGE_SEVERITY_CORRUPTION");

				if (WindowsApi::Failed(infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true)))
					this->log->Error("SetBreakOnSeverity - D3D11_MESSAGE_SEVERITY_ERROR");
#endif
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS
				};

				D3D11_INFO_QUEUE_FILTER filter
				{
					.DenyList
					{
						.NumIDs = static_cast<UINT>(std::size(hide)),
						.pIDList = hide
					}
				};

				if (WindowsApi::Failed(infoQueue->AddStorageFilterEntries(&filter)))
					this->log->Error("AddStorageFilterEntries");
			}
			else
				this->log->Error("ID3D11InfoQueue");
		}
		else
			this->log->Error("ID3D11Debug");
#endif
	}

	void Direct3D11Video::ReportDebug()
	{
#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D11Debug> debug;
		if (WindowsApi::Succeeded(this->device.As(&debug)))
		{
			if (WindowsApi::Failed(debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL)))
				this->log->Error("ReportLiveDeviceObjects");
		}
		else
			this->log->Error("ID3D11Debug");
#endif
	}

	void Direct3D11Video::UpdateColorSpace()
	{
		if (!this->dxgiFactory)
			return;

		if (!this->dxgiFactory->IsCurrent())
		{
			// Output information is cached on the DXGI Factory. If it is stale we need to create a new factory.
			this->CreateFactory();
		}

		// To detect HDR support, we will need to check the color space in the primary DXGI output associated with the app at this point in time (using window/display intersection).
		auto colorSpace{DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709};
		bool isDisplayHDR10{};
		if (this->swapChain)
		{
			RECT windowBounds{};
			if (!GetWindowRect(WindowsApi::GetHandleWindow(), &windowBounds))
				this->log->Error("GetWindowRect");

			const auto x1{windowBounds.left};
			const auto y1{windowBounds.top};
			const auto x2{windowBounds.right};
			const auto y2{windowBounds.bottom};
			auto bestIntersectArea{-1l};

			Microsoft::WRL::ComPtr<IDXGIOutput> bestOutput;
			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
			for (auto adapterIndex{0}; WindowsApi::Succeeded(this->dxgiFactory->EnumAdapters(adapterIndex, adapter.ReleaseAndGetAddressOf())); ++adapterIndex)
			{
				Microsoft::WRL::ComPtr<IDXGIOutput> output;
				for (auto outputIndex{0}; WindowsApi::Succeeded(adapter->EnumOutputs(outputIndex, output.ReleaseAndGetAddressOf())); ++outputIndex)
				{
					DXGI_OUTPUT_DESC desc{};
					if (WindowsApi::Failed(output->GetDesc(&desc)))
						this->log->Error("GetDesc");

					const auto& rect{desc.DesktopCoordinates};
					const auto intersectArea{Direct3D11Video::ComputeIntersectionArea(x1, y1, x2, y2, rect.left, rect.top, rect.right, rect.bottom)};
					if (intersectArea > bestIntersectArea)
					{
						bestOutput.Swap(output);
						bestIntersectArea = intersectArea;
					}
				}
			}

			if (bestOutput)
			{
				Microsoft::WRL::ComPtr<IDXGIOutput6> output6;
				if (WindowsApi::Succeeded(bestOutput.As(&output6)))
				{
					DXGI_OUTPUT_DESC1 desc{};
					if (WindowsApi::Failed(output6->GetDesc1(&desc)))
						this->log->Error("GetDesc");

					if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
						isDisplayHDR10 = true;
				}
				else
					this->log->Error("IDXGIOutput");
			}
		}

		if (this->enableHDR && isDisplayHDR10)
		{
			switch (this->backBufferFormat)
			{
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				// The application creates the HDR10 signal.
				colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
				break;

			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				// The system creates the HDR10 signal; application uses linear values.
				colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
				break;

			default:
				break;
			}
		}

		Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain3;
		if (this->swapChain && WindowsApi::Succeeded(this->swapChain.As(&swapChain3)))
		{
			UINT colorSpaceSupport{};
			if (WindowsApi::Succeeded(swapChain3->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport)) && (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
			{
				if (WindowsApi::Failed(swapChain3->SetColorSpace1(colorSpace)))
					this->log->Error("SetColorSpace");
			}
			else
				this->log->Error("CheckColorSpaceSupport");
		}
		else
			this->log->Error("IDXGISwapChain");
	}

	void Direct3D11Video::HandleDeviceLost()
	{
		if (this->deviceNotify)
			this->deviceNotify->OnDeviceLost();

		this->Cleanup();

		this->CreateDeviceResources();
		this->CreateWindowSizeDependentResources();

		if (this->deviceNotify)
			this->deviceNotify->OnDeviceRestored();
	}

	bool Direct3D11Video::WindowSizeChanged(std::optional<int> width, std::optional<int> height)
	{
		const auto newWidth{width.value_or(this->videoConfiguration.Width)};
		const auto newHeight{height.value_or(this->videoConfiguration.Height)};
		if (newWidth == this->videoConfiguration.Width && newHeight == this->videoConfiguration.Height)
		{
			this->UpdateColorSpace();
			return false;
		}

		if (width.has_value())
			this->videoConfiguration.Width = width.value();

		if (height.has_value())
			this->videoConfiguration.Height = height.value();

		this->CreateWindowSizeDependentResources();
		return true;
	}

	void Direct3D11Video::Clear(const Color& clearColor)
	{
		this->context->ClearRenderTargetView(this->renderTargetView.Get(), &clearColor.GetFloat4()[0]);
		this->context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		this->context->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());
		this->context->RSSetViewports(1, &this->screenViewport);
	}

	void Direct3D11Video::Present()
	{
		auto result{E_FAIL};
		if (this->allowTearing)
		{
			// Recommended to always use tearing if supported when using a sync interval of 0.
			result = this->swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
		}
		else
		{
			// The first argument instructs DXGI to block until VSync, putting the application to sleep until the next VSync. This ensures we don't waste any cycles rendering frames that will never be displayed to the screen.
			result = this->swapChain->Present(1, 0);
		}

		// Discard the contents of the render target. This is a valid operation only when the existing contents will be entirely overwritten. If dirty or scroll rects are used, this call should be removed.
		this->context->DiscardView(this->renderTargetView.Get());

		if (this->depthStencilView)
		{
			// Discard the contents of the depth stencil.
			this->context->DiscardView(this->depthStencilView.Get());
		}

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
		{
			const auto code{static_cast<unsigned int>((result == DXGI_ERROR_DEVICE_REMOVED) ? this->device->GetDeviceRemovedReason() : result)};
			this->log->Warning(std::format("Device Lost on Present: {}", code));
			this->HandleDeviceLost();
		}
		else
		{
			if (WindowsApi::Failed(result))
				this->log->Error("Present");

			if (!this->dxgiFactory->IsCurrent())
				this->UpdateColorSpace();
		}
	}

	void Direct3D11Video::Render(const Color& clearColor)
	{
		this->Clear(clearColor);
		this->_Render_();
		this->Present();
	}

	constexpr DXGI_FORMAT Direct3D11Video::NoSRGB(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8X8_UNORM;
		default:                                return format;
		}
	}

	constexpr long Direct3D11Video::ComputeIntersectionArea(long ax1, long ay1, long ax2, long ay2, long bx1, long by1, long bx2, long by2)
	{
		return std::max(0l, std::min(ax2, bx2) - std::max(ax1, bx1)) * std::max(0l, std::min(ay2, by2) - std::max(ay1, by1));
	}

	void Direct3D11Video::_Render_()
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
		};

		constexpr Vertex vertexData[6] =
		{
			{ { 0.0f,   0.5f,  0.5f/*, 1.0f*/ },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top / Red
			{ { 0.5f,  -0.5f,  0.5f/*, 1.0f*/ },{ 0.0f, 1.0f, 0.0f, 1.0f } },  // Right / Green
			{ { -0.5f, -0.5f,  0.5f/*, 1.0f*/ },{ 0.0f, 0.0f, 1.0f, 1.0f } },   // Left / Blue

			{ { 0.0f,   0.5f,  0.5f/*, 1.0f*/ },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top / Red
			{ { 1.0f,  0.5f,  0.5f/*, 1.0f*/ },{ 0.0f, 0.0f, 1.0f, 1.0f } },  // Right / Green
			{ { 0.5f, -0.5f,  0.5f/*, 1.0f*/ },{ 0.0f, 1.0f, 0.0f, 1.0f } }   // Left / Blue
		};

		this->vertexBuffers.Get("Dynamic")->MapData(vertexData, this->context.Get());

		this->shaders.Get("VertexShader")->SetShader(this->context.Get());
		this->shaders.Get("PixelShader")->SetShader(this->context.Get());
		this->vertexBuffers.Get("Dynamic")->SetVertexBuffer(sizeof(Vertex), this->context.Get());
		this->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->context->Draw(6, 0);
	}
}