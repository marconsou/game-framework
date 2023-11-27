export module Video;

import Color;

export namespace gfl
{
	class Video
	{
	public:
		Video() = default;
		Video(const Video&) = delete;
		Video(Video&&) = delete;
		Video& operator=(const Video&) = delete;
		Video& operator=(Video&&) = delete;
		virtual ~Video() = default;
		virtual void Render(const Color& clearColor) = 0;
	};
}