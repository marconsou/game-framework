export module AppConfiguration;

import std;

export namespace gfl
{
	struct AppConfiguration
	{
		std::string Title;
		int Width{};
		int Height{};
		bool Windowed{};
		bool Resize{};
		bool ShowCursor{};
		int ResourceIconId{};
	};
}