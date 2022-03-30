#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>

#include <string>
#include <array>

#include "Vector.h"
#include "Colour.h"
#include "FileUtils.h"

namespace Run {
	constexpr uint32_t MAX_KEYS = 500;

	class Window
	{
	public:
		static Window& get();
		void setSize(int width, int height);
		void setWidth(int width);
		void setHeight(int height);
		void setPosition(int x, int y);
		void setTitle(std::string& title);
		void setTitle(std::string&& title);

		void center();

		void getInputEvents();

		void destroy();

		bool shouldClose();
		bool isFullscreen();

		bool isKeyDown(uint32_t key);
		bool isKeyUp(uint32_t key);
		bool isKeyJustDown(uint32_t key);
		bool isKeyJustUp(uint32_t key);

		Math::Vector2 getMousePosition();
		void setMousePosition(int x, int y);
		
		void setCursor(int cursor);
		void setCursor(Cursor cursor, int hotX = 0, int hotY = 0);
		void defaultCursor();

		void setColour(Colour colour);
		Colour getColour();

		void setFullscreen(bool fullscreen);

		Math::Vector2 getSize();
		Math::Vector2 getPosition();

		// Dont use
		bool isFramebufferResized();
		void setFramebufferResizedFalse();

		GLFWwindow*& getGLFWwindow();

		Window(Window&) = delete;
	private:
		Window();

		static void windowMovedCallback(GLFWwindow* window, int xpos, int ypos);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		static Window* m_instance;

		GLFWwindow* m_window = nullptr;
		std::string m_title = "";
		Math::Vector2 m_size{ 1280, 720 };
		Math::Vector2 m_position{ 100, 100 };

		GLFWcursor* m_cursor = nullptr;

		bool m_fullscreen = false;
		bool m_framebufferResized = false;

		Math::Vector4 m_colour{0.0f, 0.0f, 0.0f, 0.0f};

		struct {
			std::array<bool, MAX_KEYS> keys;
			std::array<bool, MAX_KEYS> keysJustPressed;
			Math::Vector2 mousePos{0.0f, 0.0f};
		} m_input;
	};
}

