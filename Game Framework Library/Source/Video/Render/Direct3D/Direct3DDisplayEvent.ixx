export module Direct3DDisplayEvent;

import Direct3DVideo;
import DisplayNotify;
import VideoNotify;

export namespace gfl
{
	class Direct3DDisplayEvent : public DisplayNotify
	{
	public:
		Direct3DDisplayEvent(Direct3DVideo* direct3DVideo);
		void SetVideoNotify(VideoNotify* videoNotify);
		void OnWindowSizeChanged(int width, int height) override;
		void OnWindowMoved() override;
		void OnDisplayChange() override;
	private:
		Direct3DVideo* direct3DVideo{};
		VideoNotify* videoNotify{};
	};
}