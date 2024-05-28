module;

#include "Direct3D11.h"

export module Direct3D11Video;

import Video;
import VideoConfiguration;
import VideoNotify;
//import "Direct3D11.h";

export namespace gfl
{
	class Direct3D11Video : public Video
	{
	public:
		Direct3D11Video(const VideoConfiguration& videoConfiguration, VideoNotify* videoNotify);
		~Direct3D11Video();
		void Render(const Color& clearColor) override;
	};
}