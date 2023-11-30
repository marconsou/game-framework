export module GameApp;

import Game;

import SystemFactory;
import SystemFactoryWindows;

export class GameApp : public gfl::Game
{
public:
	GameApp()
	{
		std::unique_ptr<gfl::SystemFactory> systemFactory = std::make_unique<gfl::SystemFactoryWindows>();
		const gfl::AppConfiguration appConfiguration{.Title = "Game App",.Width = 640,.Height = 480,.Windowed = true,.ShowCursor = true,.ResourceIconId = 0};
		const gfl::VideoConfiguration videoConfiguration{.VSync = false};
		this->App = systemFactory->CreateApp(appConfiguration, this);
		this->Video = systemFactory->CreateVideo(videoConfiguration, gfl::VideoSystem::Direct3D12, this);
		this->Input = systemFactory->CreateInput();

		this->CreateDeviceDependentResources();
		this->CreateWindowSizeDependentResources();

		// TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
		//   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
		//   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
		//   Add DX::DeviceResources::c_ReverseDepth to optimize depth buffer clears for 0 instead of 1.
	}
};