export module MouseWindows;

import <unordered_map>;
import "DirectXTKMouse.h";
import Mouse;
import MouseButton;
import WindowsApi;

export namespace gfl
{
	class MouseWindows : public Mouse
	{
	public:
		MouseWindows()
		{
			this->device.SetWindow(WindowsApi::GetHandleWindow());
			this->states[InputState::Pressed] = DirectX::Mouse::ButtonStateTracker::PRESSED;
			this->states[InputState::Released] = DirectX::Mouse::ButtonStateTracker::RELEASED;
			this->states[InputState::Holding] = DirectX::Mouse::ButtonStateTracker::HELD;
		}

		int GetX() const override
		{
			return this->device.GetState().x;
		}

		int GetY() const override
		{
			return this->device.GetState().y;
		}

		int GetScrollWheel() const override
		{
			return this->device.GetState().scrollWheelValue;
		}

		bool IsMouseButton(MouseButton button, InputState state) override
		{
			return (this->buttons[button] == this->states[state]);
		}

		void Update() override
		{
			this->tracker.Update(this->device.GetState());

			this->buttons[MouseButton::Left] = this->tracker.leftButton;
			this->buttons[MouseButton::Middle] = this->tracker.middleButton;
			this->buttons[MouseButton::Right] = this->tracker.rightButton;
			this->buttons[MouseButton::XButton1] = this->tracker.xButton1;
			this->buttons[MouseButton::XButton2] = this->tracker.xButton2;
		}

		static void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
		{
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		}
	private:
		DirectX::Mouse device;
		DirectX::Mouse::ButtonStateTracker tracker;
		std::unordered_map<MouseButton, DirectX::Mouse::ButtonStateTracker::ButtonState> buttons;
		std::unordered_map<InputState, DirectX::Mouse::ButtonStateTracker::ButtonState> states;
	};
}