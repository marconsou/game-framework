export module VideoConfiguration;

export namespace gfl
{
	struct VideoConfiguration
	{
		int Width{};
		int Height{};
		bool Windowed{};
		bool VSync{};
	};
}