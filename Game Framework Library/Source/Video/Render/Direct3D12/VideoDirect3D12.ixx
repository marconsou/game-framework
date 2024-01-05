export module VideoDirect3D12;

import "DirectX12.h";
import DeviceStateNotification;
import Video;
import VideoConfiguration;

export namespace gfl
{
	class VideoDirect3D12 : public Video
	{
	public:
		VideoDirect3D12(const VideoConfiguration& videoConfiguration, DeviceStateNotification* deviceStateNotification);
		~VideoDirect3D12();
		void Render(const Color& clearColor) override;
		bool OnWindowSizeChanged(int width, int height) override;
		void OnWindowMoved() override;
		void OnDisplayChange() override;
	private:
		bool allowTearing{true};
		const bool enableHDR{};
		const bool reverseDepth{};
		const UINT backBufferCount{2};
		UINT backBufferIndex{};
		UINT rtvDescriptorSize{};
		DWORD factoryFlags{};
		std::vector<UINT64> fenceValues;
		const DXGI_FORMAT backBufferFormat{DXGI_FORMAT_B8G8R8A8_UNORM};
		const DXGI_FORMAT depthBufferFormat{DXGI_FORMAT_D32_FLOAT};
		const D3D_FEATURE_LEVEL minFeatureLevel{D3D_FEATURE_LEVEL_11_0};
		D3D_FEATURE_LEVEL featureLevel{D3D_FEATURE_LEVEL_11_0};
		DXGI_COLOR_SPACE_TYPE colorSpace{DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709};
		D3D12_VIEWPORT screenViewport{};
		D3D12_RECT scissorRect{};
		Microsoft::WRL::Wrappers::Event fenceEvent;
		Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
		std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> commandAllocators;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencil;
		DeviceStateNotification* deviceStateNotification{};
		VideoConfiguration videoConfiguration;
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const;
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void CheckForCPUFeatureSupport();
		void EnableDebugLog();
		void ReportDebugLog();
		void CreateFactory();
		void CheckForAllowTearingSupport();
		void CreateDevice();
		void GetAdapter(IDXGIAdapter1** ppAdapter);
		void ConfigureDebugLog();
		void CheckForFeatureLevelSupport();
		void CreateCommandQueue();
		void CreateDescriptorHeap();
		void CreateCommandAllocator();
		void CreateCommandList();
		void CreateFence();
		void CheckForShaderModelSupport();
		void WaitForGpu();
		void MoveToNextFrame();
		void CreateSwapChain();
		void HandleDeviceLost();
		void UpdateColorSpace();
		void CreateRenderTargetView();
		void ResetBackBufferIndex();
		void CreateDepthStencilView();
		void ViewportScissorSizeUpdate();
		bool WindowSizeChanged(int width, int height);
		void Prepare(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES afterState = D3D12_RESOURCE_STATE_RENDER_TARGET);
		void Clear(const Color& clearColor);
		void Present(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_RENDER_TARGET);
		constexpr DXGI_FORMAT NoSRGB(DXGI_FORMAT format);
		constexpr long ComputeIntersectionArea(long ax1, long ay1, long ax2, long ay2, long bx1, long by1, long bx2, long by2);
	};
}