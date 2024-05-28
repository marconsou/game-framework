export module WindowsInput;

import Input;
import WindowsKeyboard;
import WindowsMouse;

export namespace gfl
{
	class WindowsInput : public Input
	{
	public:
		WindowsInput() = default;
		int GetMouseX() const override;
		int GetMouseY() const override;
		int GetMouseScrollWheel() const override;
		bool IsKeyboardKey(KeyboardKey key, InputState state) override;
		bool IsMouseButton(MouseButton button, InputState state) override;
		void Update() override;
	private:
		WindowsKeyboard keyboard;
		WindowsMouse mouse;
	};
}