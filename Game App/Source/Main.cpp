import std;
import EntryPoint;
import Game;

int gfl::EntryPoint::Main()
{
	std::unique_ptr<gfl::Game> game = std::make_unique<gfl::Game>();
	return game->windowsApp->Run();
}