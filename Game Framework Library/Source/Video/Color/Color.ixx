export module Color;

import std;

export namespace gfl
{
	class Color
	{
	public:
		float Red{};
		float Green{};
		float Blue{};
		float Alpha{};
		bool IsOpaque() const;
		std::array<float, 4> GetFloat4() const;
	};
}