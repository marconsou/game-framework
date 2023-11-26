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

		int GetMouseX() const override
		{
			return this->mouse.GetX();
		}

		int GetMouseY() const override
		{
			return this->mouse.GetY();
		}

		int GetMouseScrollWheel() const override
		{
			return this->mouse.GetScrollWheel();
		}

		bool IsKeyboardKey(KeyboardKey key, InputState state) override
		{
			return this->keyboard.IsKeyboardKey(key, state);
		}

		bool IsMouseButton(MouseButton button, InputState state) override
		{
			return this->mouse.IsMouseButton(button, state);
		}

		void Update() override
		{
			this->keyboard.Update();
			this->mouse.Update();
		}
	private:
		KeyboardWindows keyboard;
		MouseWindows mouse;
	};
}