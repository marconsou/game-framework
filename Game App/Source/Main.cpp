import <format>;
import ColorPalette;
import EntryPoint;
import GameApp;

import DateTime;

using namespace std;
using namespace gfl;

int EntryPoint::Main()
{
	const auto date = DateTime::GetDate();
	GameApp game;
	return game.App->OnRun([&]
	{ 
		game.Input->Update();

		game.App->SetTitle(format("{}fps {:.1f}s", game.GlobalTimer.GetFramesPerSecond(), game.GlobalTimer.GetTotalSeconds()));

		if (game.Input->IsKeyboardKey(KeyboardKey::Escape, InputState::Released) || 
			game.Input->IsMouseButton(MouseButton::Right, InputState::Released))
			game.App->Quit();

		game.Run(ColorPalette::SkyBlue);
	});
}