export module Color;

import <array>;

export namespace gfl
{
	class Color
	{
	public:
		float Red{};
		float Green{};
		float Blue{};
		float Alpha{};

		constexpr bool IsOpaque() const
		{
			return this->Alpha == 1.0f;
		}

		constexpr std::array<float, 4> GetFloat4() const
		{
			return {this->Red, this->Green, this->Blue, this->Alpha};
		}
	};
}