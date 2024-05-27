module;

#include "../../../App/Windows/WindowsCompact.h"
#include "Direct3D11.h"

export module Direct3D11Video;

import DeviceNotify;
import Video;
import VideoConfiguration;

export namespace gfl
{
	class Direct3D11Video : Video
	{
	public:
		Direct3D11Video(const VideoConfiguration& videoConfiguration, DeviceNotify* deviceNotify);
		~Direct3D11Video();
		void Render(const Color& clearColor) override;
	};
}