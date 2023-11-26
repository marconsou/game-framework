export module Mouse;

import InputState;
import MouseButton;

export namespace gfl
{
	class Mouse
	{
	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse(Mouse&&) = delete;
		Mouse& operator=(const Mouse&) = delete;
		Mouse& operator=(Mouse&&) = delete;
		virtual ~Mouse() = default;
		virtual int GetX() const = 0;
		virtual int GetY() const = 0;
		virtual int GetScrollWheel() const = 0;
		virtual bool IsMouseButton(MouseButton button, InputState state) = 0;
		virtual void Update() = 0;
	};
}