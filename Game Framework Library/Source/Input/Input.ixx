export module Input;

import InputState;
import KeyboardKey;
import MouseButton;

export namespace gfl
{
	class Input
	{
	public:
		Input() = default;
		Input(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(const Input&) = delete;
		Input& operator=(Input&&) = delete;
		virtual ~Input() = default;
		virtual int GetMouseX() const = 0;
		virtual int GetMouseY() const = 0;
		virtual int GetMouseScrollWheel() const = 0;
		virtual bool IsKeyboardKey(KeyboardKey key, InputState state) = 0;
		virtual bool IsMouseButton(MouseButton button, InputState state) = 0;
		virtual void Update() = 0;
	};
}