export module Keyboard;

import InputState;
import KeyboardKey;

export namespace gfl
{
	class Keyboard
	{
	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard(Keyboard&&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		Keyboard& operator=(Keyboard&&) = delete;
		virtual ~Keyboard() = default;
		virtual bool IsKeyboardKey(KeyboardKey key, InputState state) = 0;
		virtual void Update() = 0;
	};
}