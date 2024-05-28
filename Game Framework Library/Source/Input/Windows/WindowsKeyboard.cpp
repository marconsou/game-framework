module WindowsKeyboard;

import KeyboardKey;

namespace gfl
{
	WindowsKeyboard::WindowsKeyboard()
	{
		using Key = DirectX::Keyboard::Keys;

		this->keys[KeyboardKey::A] = Key::A;
		this->keys[KeyboardKey::B] = Key::B;
		this->keys[KeyboardKey::C] = Key::C;
		this->keys[KeyboardKey::D] = Key::D;
		this->keys[KeyboardKey::E] = Key::E;
		this->keys[KeyboardKey::F] = Key::F;
		this->keys[KeyboardKey::G] = Key::G;
		this->keys[KeyboardKey::H] = Key::H;
		this->keys[KeyboardKey::I] = Key::I;
		this->keys[KeyboardKey::J] = Key::J;
		this->keys[KeyboardKey::K] = Key::K;
		this->keys[KeyboardKey::L] = Key::L;
		this->keys[KeyboardKey::M] = Key::M;
		this->keys[KeyboardKey::N] = Key::N;
		this->keys[KeyboardKey::O] = Key::O;
		this->keys[KeyboardKey::P] = Key::P;
		this->keys[KeyboardKey::Q] = Key::Q;
		this->keys[KeyboardKey::R] = Key::R;
		this->keys[KeyboardKey::S] = Key::S;
		this->keys[KeyboardKey::T] = Key::T;
		this->keys[KeyboardKey::U] = Key::U;
		this->keys[KeyboardKey::V] = Key::V;
		this->keys[KeyboardKey::W] = Key::W;
		this->keys[KeyboardKey::X] = Key::X;
		this->keys[KeyboardKey::Y] = Key::Y;
		this->keys[KeyboardKey::Z] = Key::Z;
		this->keys[KeyboardKey::F1] = Key::F1;
		this->keys[KeyboardKey::F2] = Key::F2;
		this->keys[KeyboardKey::F3] = Key::F3;
		this->keys[KeyboardKey::F4] = Key::F4;
		this->keys[KeyboardKey::F5] = Key::F5;
		this->keys[KeyboardKey::F6] = Key::F6;
		this->keys[KeyboardKey::F7] = Key::F7;
		this->keys[KeyboardKey::F8] = Key::F8;
		this->keys[KeyboardKey::F9] = Key::F9;
		this->keys[KeyboardKey::F10] = Key::F10;
		this->keys[KeyboardKey::F11] = Key::F11;
		this->keys[KeyboardKey::F12] = Key::F12;
		this->keys[KeyboardKey::Key0] = Key::D0;
		this->keys[KeyboardKey::Key1] = Key::D1;
		this->keys[KeyboardKey::Key2] = Key::D2;
		this->keys[KeyboardKey::Key3] = Key::D3;
		this->keys[KeyboardKey::Key4] = Key::D4;
		this->keys[KeyboardKey::Key5] = Key::D5;
		this->keys[KeyboardKey::Key6] = Key::D6;
		this->keys[KeyboardKey::Key7] = Key::D7;
		this->keys[KeyboardKey::Key8] = Key::D8;
		this->keys[KeyboardKey::Key9] = Key::D9;
		this->keys[KeyboardKey::NumPad0] = Key::NumPad0;
		this->keys[KeyboardKey::NumPad1] = Key::NumPad1;
		this->keys[KeyboardKey::NumPad2] = Key::NumPad2;
		this->keys[KeyboardKey::NumPad3] = Key::NumPad3;
		this->keys[KeyboardKey::NumPad4] = Key::NumPad4;
		this->keys[KeyboardKey::NumPad5] = Key::NumPad5;
		this->keys[KeyboardKey::NumPad6] = Key::NumPad6;
		this->keys[KeyboardKey::NumPad7] = Key::NumPad7;
		this->keys[KeyboardKey::NumPad8] = Key::NumPad8;
		this->keys[KeyboardKey::NumPad9] = Key::NumPad9;
		this->keys[KeyboardKey::Escape] = Key::Escape;
		this->keys[KeyboardKey::Tab] = Key::Tab;
		this->keys[KeyboardKey::CapsLock] = Key::CapsLock;
		this->keys[KeyboardKey::Backspace] = Key::Back;
		this->keys[KeyboardKey::Enter] = Key::Enter;
		this->keys[KeyboardKey::Spacebar] = Key::Space;
		this->keys[KeyboardKey::LeftShift] = Key::LeftShift;
		this->keys[KeyboardKey::RightShift] = Key::RightShift;
		this->keys[KeyboardKey::LeftControl] = Key::LeftControl;
		this->keys[KeyboardKey::RightControl] = Key::RightControl;
		this->keys[KeyboardKey::LeftAlt] = Key::LeftAlt;
		this->keys[KeyboardKey::RightAlt] = Key::RightAlt;
		this->keys[KeyboardKey::LeftSystem] = Key::LeftWindows;
		this->keys[KeyboardKey::RightSystem] = Key::RightWindows;
		this->keys[KeyboardKey::Menu] = Key::Apps;
		this->keys[KeyboardKey::ScrollLock] = Key::Scroll;
		this->keys[KeyboardKey::PauseBreak] = Key::Pause;
		this->keys[KeyboardKey::Insert] = Key::Insert;
		this->keys[KeyboardKey::Delete] = Key::Delete;
		this->keys[KeyboardKey::Home] = Key::Home;
		this->keys[KeyboardKey::End] = Key::End;
		this->keys[KeyboardKey::PageUp] = Key::PageUp;
		this->keys[KeyboardKey::PageDown] = Key::PageDown;
		this->keys[KeyboardKey::Left] = Key::Left;
		this->keys[KeyboardKey::Up] = Key::Up;
		this->keys[KeyboardKey::Right] = Key::Right;
		this->keys[KeyboardKey::Down] = Key::Down;
		this->keys[KeyboardKey::NumLock] = Key::NumLock;
		this->keys[KeyboardKey::Divide] = Key::Divide;
		this->keys[KeyboardKey::Multiply] = Key::Multiply;
		this->keys[KeyboardKey::Subtract] = Key::Subtract;
		this->keys[KeyboardKey::Add] = Key::Add;
		this->keys[KeyboardKey::Decimal] = Key::Decimal;
		this->keys[KeyboardKey::GraveAccent] = Key::OemTilde;
		this->keys[KeyboardKey::Minus] = Key::OemMinus;
		this->keys[KeyboardKey::Equal] = Key::OemPlus;
		this->keys[KeyboardKey::OpenBracket] = Key::OemOpenBrackets;
		this->keys[KeyboardKey::CloseBracket] = Key::OemCloseBrackets;
		this->keys[KeyboardKey::Slash] = Key::OemPipe;
		this->keys[KeyboardKey::Semicolon] = Key::OemSemicolon;
		this->keys[KeyboardKey::SingleQuote] = Key::OemQuotes;
		this->keys[KeyboardKey::Comma] = Key::OemComma;
		this->keys[KeyboardKey::Period] = Key::OemPeriod;
		this->keys[KeyboardKey::Backslash] = Key::OemQuestion;
	}

	bool WindowsKeyboard::IsKeyboardKey(KeyboardKey key, InputState state)
	{
		return this->states[state](this->keys[key]);
	}

	void WindowsKeyboard::Update()
	{
		this->tracker.Update(this->device.GetState());

		this->states[InputState::Pressed] = [&](auto key) -> auto { return this->tracker.IsKeyPressed(key); };
		this->states[InputState::Released] = [&](auto key) -> auto { return this->tracker.IsKeyReleased(key); };
		this->states[InputState::Holding] = [&](auto key) -> auto { return this->device.GetState().IsKeyDown(key); };
	}

	void WindowsKeyboard::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
	}
}