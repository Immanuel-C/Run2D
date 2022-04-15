#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>

#include <string>
#include <array>

#include "Vector.h"
#include "Colour.h"
#include "FileUtils.h"
#include "InputMapper.h"
#include "DynamicExport.h"

namespace Run {

	class RUN_API Window
	{
	public:
		static Window& get();
		Window& setSize(int width, int height);
		Window& setWidth(int width);
		Window& setHeight(int height);
		Window& setPosition(int x, int y);
		Window& setTitle(std::string& title);
		Window& setTitle(std::string&& title);

		void center();

		void getInputEvents();

		void destroy();

		bool shouldClose();
		bool isFullscreen();

		template <typename... Args>
		bool isKeyDown(Keys key, Args ...args) { return isKeyDown(key) && isKeyDown(args...); }
		template <typename... Args>
		bool isKeyUp(Keys key, Args ...args) { return !isKeyUp(key) && !isKeyUp(args...); }
		template <typename... Args>
		bool isKeyJustDown(Keys key, Args ...args) { return isKeyJustDown(key) && isKeyJustDown(args...); }
		template <typename... Args>
		bool isKeyJustUp(Keys key, Args ...args) { return isKeyJustUp(key) && isKeyJustUp(args...); }

		bool isKeyDown(Keys key);
		bool isKeyUp(Keys key);
		bool isKeyJustDown(Keys key);
		bool isKeyJustUp(Keys key);

		bool isMouseButtonDown(MouseButtons button);
		bool isMouseButtonUp(MouseButtons button);

		Math::Vector2 getMousePosition();
		Window& setMousePosition(int x, int y);
		
		Window& setCursor(int cursor);
		// Automatically deletes the cursor
		Window& setCursor(Cursor cursor, int hotX = 0, int hotY = 0);
		void defaultCursor();

		Window& setColour(Colour colour);
		Colour getColour();

		Window& setFullscreen(bool fullscreen);

		// Automatically deletes the icon
		Window& setIcon(Icon icon);

		Math::Vector2 getSize();
		Math::Vector2 getPosition();

		bool isGamepadButtonPressed(GamepadID jid, GamepadButtons button);
		float getGamepadAxis(GamepadID jid, GamepadAxis axis);

		bool isGamepadConnected(GamepadID jid);

		std::vector<const char*> getDroppedPaths();
		bool pathsJustDropped();

		std::string getClipboardString();
		Window& setClipboardString(const std::string& cpbStr);

		// Dont use
		bool isFramebufferResized();
		// Dont use
		void setFramebufferResizedFalse();

		float getOpacity();
		Window& setOpacity(float amount);

		bool isVisible();
		Window& setVisibility(bool visible);

		GLFWwindow*& getGLFWwindow();

		Window(Window&) = delete;
	private:
		Window();

		static void windowMovedCallback(GLFWwindow* window, int xpos, int ypos);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		static void windowFocusCallback(GLFWwindow* window, int focused);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void joystickCallback(int jid, int event);
		static void windowDropCallback(GLFWwindow* window, int count, const char** paths);


		static Window* m_instance;

		GLFWwindow* m_window = nullptr;
		std::string m_title = "";
		Math::Vector2 m_size{ 1280, 720 };
		Math::Vector2 m_position{ 100, 100 };

		GLFWcursor* m_cursor = nullptr;

		bool m_fullscreen = false;
		bool m_framebufferResized = false;
		bool m_focused = false;
		bool m_visible = true;

		Colour m_colour{0.0f, 0.0f, 0.0f, 0.0f};

		float m_opacity = 1.0f;


		std::vector<const char*> m_dropPaths;
		bool m_pathsJustDropped = false;

		static constexpr uint32_t MAX_KEYS = 500;
		static constexpr uint32_t MAX_MOUSE_BUTTONS = 10;
		static constexpr uint32_t MAX_GAMEPADS = 16;

		struct {
			std::array<bool, MAX_KEYS> keys;
			std::array<bool, MAX_KEYS> keysJustPressed;
			std::array<bool, MAX_MOUSE_BUTTONS> mouseButtons;
			std::array<bool, MAX_GAMEPADS> gamePadsConnected;
			Math::Vector2 mousePos{0.0f, 0.0f};
		} m_input;
	};
}

