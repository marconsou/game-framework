module Direct3DDisplayEvent;

namespace gfl
{
	Direct3DDisplayEvent::Direct3DDisplayEvent(Direct3DVideo* direct3DVideo) : direct3DVideo{direct3DVideo}
	{

	}

	void Direct3DDisplayEvent::SetVideoNotify(VideoNotify* videoNotify)
	{
		this->videoNotify = videoNotify;
	}

	void Direct3DDisplayEvent::OnWindowSizeChanged(int width, int height)
	{
		if (!this->direct3DVideo->WindowSizeChanged(width, height))
			return;

		if (this->videoNotify)
			this->videoNotify->OnResize(width, height);
	}

	void Direct3DDisplayEvent::OnWindowMoved()
	{
		this->direct3DVideo->WindowSizeChanged();
	}

	void Direct3DDisplayEvent::OnDisplayChange()
	{
		this->direct3DVideo->UpdateColorSpace();
	}
}