#include <DirectXColors.h>

import <memory>;
import EntryPoint;
import App;
import AppWindows;
import AppConfiguration;
import AppNotification;
import CrtDebugLogWindows;
import DateTime;
import Log;
import WindowsApi;
import Clock;
import Timer;
import ColorPalette;
import SystemFactory;
import SystemFactoryWindows;
import VideoConfiguration;
import VideoNotification;
import Game;
import GameApp;

using namespace std;
using namespace gfl;

int EntryPoint::Main()
{
	Game game;
	unique_ptr<SystemFactory> systemFactory = make_unique<SystemFactoryWindows>();
	const AppConfiguration appConfiguration{.Title = "Game App",.Width = 640,.Height = 480,.Windowed = true,.ShowCursor = true,.ResourceIconId = 0};
	const VideoConfiguration videoConfiguration{.VSync = false};
	const auto app = systemFactory->CreateApp(appConfiguration, &game, &game);

	game.Initialize(WindowsApi::GetHandleWindow(), appConfiguration.Width, appConfiguration.Height);

	auto date = DateTime::GetDate();
	auto time = DateTime::GetTime();
	auto clock = Clock::CurrentTime();
	Timer timer;
	timer.GetTicks();
	//Log::Warning("Test");
	
	return app->OnRun([&] { game.Tick(); });
}