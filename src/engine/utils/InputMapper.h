#pragma once

#include <GLFW/glfw3.h>

namespace Run {
	enum struct Keys
	{
		Number0 = GLFW_KEY_0,
		Number1 = GLFW_KEY_1,
		Number2 = GLFW_KEY_2,
		Number3 = GLFW_KEY_3,
		Number4 = GLFW_KEY_4,
		Number5 = GLFW_KEY_5,
		Number6 = GLFW_KEY_6,
		Number9 = GLFW_KEY_9,
		Number7 = GLFW_KEY_7,
		Number8 = GLFW_KEY_8,

		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,

		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,

		KP0 = GLFW_KEY_KP_0,
		KP1 = GLFW_KEY_KP_1,
		KP2 = GLFW_KEY_KP_2,
		KP3 = GLFW_KEY_KP_3,
		KP4 = GLFW_KEY_KP_4,
		KP5 = GLFW_KEY_KP_5,
		KP6 = GLFW_KEY_KP_6,
		KP7 = GLFW_KEY_KP_7,
		KP8 = GLFW_KEY_KP_8,
		KP9 = GLFW_KEY_KP_9,
		KP_Decimal = GLFW_KEY_KP_DECIMAL,
		KP_Divide = GLFW_KEY_KP_DIVIDE,
		KP_Multiply = GLFW_KEY_KP_MULTIPLY,
		KP_Subtract = GLFW_KEY_KP_SUBTRACT,
		KP_Add = GLFW_KEY_KP_ADD,
		KP_Enter = GLFW_KEY_KP_ENTER,
		KP_Equal = GLFW_KEY_KP_EQUAL,

		Space = GLFW_KEY_SPACE,
		Apostrophe = GLFW_KEY_APOSTROPHE,
		Comma = GLFW_KEY_COMMA,
		Minus = GLFW_KEY_MINUS,
		Period = GLFW_KEY_PERIOD,
		Slash = GLFW_KEY_SLASH,
		Semicolon = GLFW_KEY_SEMICOLON,
		Equal = GLFW_KEY_EQUAL,
		LeftBracket = GLFW_KEY_LEFT_BRACKET,
		BackSlash = GLFW_KEY_BACKSLASH,
		RightBracket = GLFW_KEY_RIGHT_BRACKET,
		GraveAccent = GLFW_KEY_GRAVE_ACCENT, // `

		Escape = GLFW_KEY_ESCAPE,
		Enter = GLFW_KEY_ENTER,
		Tab = GLFW_KEY_TAB,
		BackSpace = GLFW_KEY_BACKSPACE,
		Insert = GLFW_KEY_INSERT,
		Delete = GLFW_KEY_DELETE,
		ArrowRight = GLFW_KEY_RIGHT,
		ArrowLeft = GLFW_KEY_LEFT,
		ArrowDown = GLFW_KEY_DOWN,
		ArrowUp = GLFW_KEY_UP,
		PageUp = GLFW_KEY_PAGE_UP,
		PageDown = GLFW_KEY_PAGE_DOWN,
		Home = GLFW_KEY_HOME,
		End = GLFW_KEY_END,
		CapsLock = GLFW_KEY_CAPS_LOCK,
		ScrollLock = GLFW_KEY_SCROLL_LOCK,
		NumberLock = GLFW_KEY_NUM_LOCK,
		PrintScreen = GLFW_KEY_PRINT_SCREEN,
		Pause = GLFW_KEY_PAUSE,
		LeftShift = GLFW_KEY_LEFT_SHIFT,
		LeftControl = GLFW_KEY_LEFT_CONTROL,
		LeftAlt = GLFW_KEY_LEFT_ALT,
		LeftWindows = GLFW_KEY_LEFT_SUPER,
		LeftSuper = GLFW_KEY_LEFT_SUPER,
		RightShift = GLFW_KEY_RIGHT_SHIFT,
		RightControl = GLFW_KEY_RIGHT_CONTROL,
		RightAlt = GLFW_KEY_RIGHT_ALT,
		RightWindows = GLFW_KEY_RIGHT_SUPER,
		RightSuper = GLFW_KEY_RIGHT_SUPER,
		Menu = GLFW_KEY_MENU,
	};

	enum struct MouseButtons
	{
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Side4 = GLFW_MOUSE_BUTTON_4,
		Side5 = GLFW_MOUSE_BUTTON_5,
		Button6 = GLFW_MOUSE_BUTTON_6,
		Button7 = GLFW_MOUSE_BUTTON_7,
	};

	enum struct GamepadButtons {
		// Xbox style
		A = GLFW_GAMEPAD_BUTTON_A,
		B = GLFW_GAMEPAD_BUTTON_B,
		X = GLFW_GAMEPAD_BUTTON_X,
		Y = GLFW_GAMEPAD_BUTTON_Y,
		LeftBumber = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
		RightBumber = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		Back = GLFW_GAMEPAD_BUTTON_BACK,
		Start = GLFW_GAMEPAD_BUTTON_START,
		Guide = GLFW_GAMEPAD_BUTTON_START,
		LeftStick = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
		RightStick = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
		DPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
		DpadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
		DpadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
		DpadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,

		// PS style
		Cross = GLFW_GAMEPAD_BUTTON_CROSS,
		Circle = GLFW_GAMEPAD_BUTTON_CIRCLE,
		Square = GLFW_GAMEPAD_BUTTON_SQUARE,
		Triangle = GLFW_GAMEPAD_BUTTON_TRIANGLE
	};

	enum struct GamepadAxis {
		LeftStickX = GLFW_GAMEPAD_AXIS_LEFT_X,
		LeftStickY = GLFW_GAMEPAD_AXIS_LEFT_Y,
		RightStickX = GLFW_GAMEPAD_AXIS_RIGHT_X,
		RightStickY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
		LeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
		RightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
	};

	enum struct GamepadID {
		GP_1  = GLFW_JOYSTICK_1,
		GP_2  = GLFW_JOYSTICK_2,
		GP_3  = GLFW_JOYSTICK_3,
		GP_4  = GLFW_JOYSTICK_4,
		GP_5  = GLFW_JOYSTICK_5,
		GP_6  = GLFW_JOYSTICK_6,
		GP_7  = GLFW_JOYSTICK_7,
		GP_8  = GLFW_JOYSTICK_8,
		GP_9  = GLFW_JOYSTICK_9,
		GP_10 = GLFW_JOYSTICK_10,
		GP_11 = GLFW_JOYSTICK_11,
		GP_12 = GLFW_JOYSTICK_12,
		GP_13 = GLFW_JOYSTICK_13,
		GP_14 = GLFW_JOYSTICK_14,
		GP_15 = GLFW_JOYSTICK_15,
		GP_16 = GLFW_JOYSTICK_16,
	};
}