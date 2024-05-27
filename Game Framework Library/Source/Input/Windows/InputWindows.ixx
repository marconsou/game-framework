export module InputWindows;

import Input;
import KeyboardWindows;
import MouseWindows;

export namespace gfl
{
	class InputWindows : public Input
	{
	public:
		InputWindows() = default;
		int GetMouseX() const override;
		int GetMouseY() const override;
		int GetMouseScrollWheel() const override;
		bool IsKeyboardKey(KeyboardKey key, InputState state) override;
		bool IsMouseButton(MouseButton button, InputState state) override;
		void Update() override;
	private:
		KeyboardWindows keyboard;
		MouseWindows mouse;
	};
}