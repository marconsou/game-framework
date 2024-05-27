module InputWindows;

namespace gfl
{
	int InputWindows::GetMouseX() const
	{
		return this->mouse.GetX();
	}

	int InputWindows::GetMouseY() const
	{
		return this->mouse.GetY();
	}

	int InputWindows::GetMouseScrollWheel() const
	{
		return this->mouse.GetScrollWheel();
	}

	bool InputWindows::IsKeyboardKey(KeyboardKey key, InputState state)
	{
		return this->keyboard.IsKeyboardKey(key, state);
	}

	bool InputWindows::IsMouseButton(MouseButton button, InputState state)
	{
		return this->mouse.IsMouseButton(button, state);
	}

	void InputWindows::Update()
	{
		this->keyboard.Update();
		this->mouse.Update();
	}
}