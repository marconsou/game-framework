module WindowsInput;

namespace gfl
{
	int WindowsInput::GetMouseX() const
	{
		return this->mouse.GetX();
	}

	int WindowsInput::GetMouseY() const
	{
		return this->mouse.GetY();
	}

	int WindowsInput::GetMouseScrollWheel() const
	{
		return this->mouse.GetScrollWheel();
	}

	bool WindowsInput::IsKeyboardKey(KeyboardKey key, InputState state)
	{
		return this->keyboard.IsKeyboardKey(key, state);
	}

	bool WindowsInput::IsMouseButton(MouseButton button, InputState state)
	{
		return this->mouse.IsMouseButton(button, state);
	}

	void WindowsInput::Update()
	{
		this->keyboard.Update();
		this->mouse.Update();
	}
}