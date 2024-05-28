module WindowsMouse;

import WindowsApi;

namespace gfl
{
	WindowsMouse::WindowsMouse()
	{
		this->device.SetWindow(WindowsApi::GetHandleWindow());
		this->states[InputState::Pressed] = DirectX::Mouse::ButtonStateTracker::PRESSED;
		this->states[InputState::Released] = DirectX::Mouse::ButtonStateTracker::RELEASED;
		this->states[InputState::Holding] = DirectX::Mouse::ButtonStateTracker::HELD;
	}

	int WindowsMouse::GetX() const
	{
		return this->device.GetState().x;
	}

	int WindowsMouse::GetY() const
	{
		return this->device.GetState().y;
	}

	int WindowsMouse::GetScrollWheel() const
	{
		return this->device.GetState().scrollWheelValue;
	}

	bool WindowsMouse::IsMouseButton(MouseButton button, InputState state)
	{
		return this->buttons[button] == this->states[state];
	}

	void WindowsMouse::Update()
	{
		this->tracker.Update(this->device.GetState());

		this->buttons[MouseButton::Left] = this->tracker.leftButton;
		this->buttons[MouseButton::Middle] = this->tracker.middleButton;
		this->buttons[MouseButton::Right] = this->tracker.rightButton;
		this->buttons[MouseButton::XButton1] = this->tracker.xButton1;
		this->buttons[MouseButton::XButton2] = this->tracker.xButton2;
	}

	void WindowsMouse::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
	}
}