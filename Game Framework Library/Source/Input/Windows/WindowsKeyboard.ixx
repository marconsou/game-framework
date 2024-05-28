export module WindowsKeyboard;

import std;
import Keyboard;
import "DirectXTKKeyboard.h";

export namespace gfl
{
	class WindowsKeyboard : public Keyboard
	{
	public:
		WindowsKeyboard();
		bool IsKeyboardKey(KeyboardKey key, InputState state) override;
		void Update() override;
		static void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	private:
		DirectX::Keyboard device;
		DirectX::Keyboard::KeyboardStateTracker tracker;
		std::unordered_map<KeyboardKey, DirectX::Keyboard::Keys> keys;
		std::unordered_map<InputState, std::function<bool(DirectX::Keyboard::Keys)>> states;
	};
}