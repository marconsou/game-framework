module;

#include "Direct3D11.h" //IntelliSense

export module Direct3D11Video;

import std;
import DeviceNotify;
import Log;
import Video;
import VideoConfiguration;
import "Direct3D11.h";

export namespace gfl
{
	class Direct3D11Video : public Video
	{
	public:
		Direct3D11Video(const VideoConfiguration& videoConfiguration, DeviceNotify* deviceNotify, Log* log);
		~Direct3D11Video();
		void Render(const Color& clearColor) override;

		//
		void UpdateColorSpace();
		bool WindowSizeChanged(std::optional<int> width = std::nullopt, std::optional<int> height = std::nullopt);
		//
	private:
		bool flipPresent{true};
		bool allowTearing{true};
		bool enableHDR{true};
		VideoConfiguration videoConfiguration;
		DeviceNotify* deviceNotify{};
		Log* log{};
		D3D_FEATURE_LEVEL featureLevel{D3D_FEATURE_LEVEL_12_1};
		const DXGI_FORMAT backBufferFormat{DXGI_FORMAT_B8G8R8A8_UNORM};
		const DXGI_FORMAT depthBufferFormat{DXGI_FORMAT_D32_FLOAT};
		const UINT backBufferCount{2};
		D3D11_VIEWPORT screenViewport{};
		Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D11Device1> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		void CheckForCPUFeatureSupport();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		UINT CheckForSDKLayerSupport();
		void CreateFactory();
		void CheckForFeaturesSupport();
		void CreateDevice(UINT creationFlags);
		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
		void CreateDebug();
		void ReportDebug();
		//void UpdateColorSpace();
		void HandleDeviceLost();
		void Clear(const Color& clearColor);
		void Present();
		//bool WindowSizeChanged(std::optional<int> width = std::nullopt, std::optional<int> height = std::nullopt);
		constexpr static DXGI_FORMAT NoSRGB(DXGI_FORMAT format);
		constexpr static long ComputeIntersectionArea(long ax1, long ay1, long ax2, long ay2, long bx1, long by1, long bx2, long by2);
	};
}