export module KeyboardWindows;

import <functional>;
import "DirectXTK12Keyboard.h";
import Keyboard;

export namespace gfl
{
	class KeyboardWindows : public Keyboard
	{
	public:
		KeyboardWindows();
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