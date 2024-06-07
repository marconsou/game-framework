module;

#include "DirectXTKMouse.h"

export module WindowsMouse;

import std;
import Mouse;

export namespace gfl
{
	class WindowsMouse : public Mouse
	{
	public:
		WindowsMouse();
		int GetX() const override;
		int GetY() const override;
		int GetScrollWheel() const override;
		bool IsMouseButton(MouseButton button, InputState state) override;
		void Update() override;
		static void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	private:
		DirectX::Mouse device;
		DirectX::Mouse::ButtonStateTracker tracker;
		std::unordered_map<MouseButton, DirectX::Mouse::ButtonStateTracker::ButtonState> buttons;
		std::unordered_map<InputState, DirectX::Mouse::ButtonStateTracker::ButtonState> states;
	};
}