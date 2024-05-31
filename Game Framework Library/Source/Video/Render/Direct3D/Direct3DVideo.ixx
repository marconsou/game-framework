export module Direct3DVideo;

import std;
import Video;

export namespace gfl
{
	class Direct3DVideo : public Video
	{
	public:
		Direct3DVideo() = default;
		Direct3DVideo(const Direct3DVideo&) = delete;
		Direct3DVideo(Direct3DVideo&&) = delete;
		Direct3DVideo& operator=(const Direct3DVideo&) = delete;
		Direct3DVideo& operator=(Direct3DVideo&&) = delete;
		virtual ~Direct3DVideo() = default;
		virtual void UpdateColorSpace() = 0;
		virtual bool WindowSizeChanged(std::optional<int> width = std::nullopt, std::optional<int> height = std::nullopt) = 0;
	};
}