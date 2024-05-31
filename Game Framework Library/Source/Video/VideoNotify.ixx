export module VideoNotify;

export namespace gfl
{
	class VideoNotify
	{
	public:
		VideoNotify() = default;
		VideoNotify(const VideoNotify&) = delete;
		VideoNotify(VideoNotify&&) = delete;
		VideoNotify& operator=(const VideoNotify&) = delete;
		VideoNotify& operator=(VideoNotify&&) = delete;
		virtual ~VideoNotify() = default;
		virtual void OnResize(int width, int height) = 0;
	};
}