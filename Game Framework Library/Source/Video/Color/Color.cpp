module Color;

namespace gfl
{
	bool Color::IsOpaque() const
	{
		return this->Alpha == 1.0f;
	}

	std::array<float, 4> Color::GetFloat4() const
	{
		return {this->Red, this->Green, this->Blue, this->Alpha};
	}
}