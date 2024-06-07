export module GameNotify;

import AppNotify;
import DeviceNotify;
import LogNotify;
import VideoNotify;

export namespace gfl
{
	class GameNotify : public AppNotify, public DeviceNotify, public VideoNotify, public LogNotify
	{
	public:
		GameNotify() = default;
		GameNotify(const GameNotify&) = delete;
		GameNotify(GameNotify&&) = delete;
		GameNotify& operator=(const GameNotify&) = delete;
		GameNotify& operator=(GameNotify&&) = delete;
		virtual ~GameNotify() = 0 {};
	};
}