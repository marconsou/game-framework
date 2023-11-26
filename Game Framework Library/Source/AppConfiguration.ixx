export module AppConfiguration;

import <string>;

export namespace gfl
{
	struct AppConfiguration
	{
		std::string Title;
		int Width{};
		int Height{};
		bool Windowed{};
		bool ShowCursor{};
		int ResourceIconId{};
	};
}