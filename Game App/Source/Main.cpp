import std;
import EntryPoint;
import GameApp;

int gfl::EntryPoint::Main()
{
	auto game = std::make_unique<GameApp>();
	return game->Start();
}