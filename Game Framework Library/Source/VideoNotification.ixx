export module VideoNotification;

export namespace gfl
{
	class VideoNotification
	{
	public:
		VideoNotification() = default;
		VideoNotification(const VideoNotification&) = delete;
		VideoNotification(VideoNotification&&) = delete;
		VideoNotification& operator=(const VideoNotification&) = delete;
		VideoNotification& operator=(VideoNotification&&) = delete;
		virtual ~VideoNotification() = default;
		virtual void OnWindowSizeChanged(int width, int height) = 0;
		virtual void OnWindowMoved() = 0;
		virtual void OnDisplayChange() = 0;
	};
}