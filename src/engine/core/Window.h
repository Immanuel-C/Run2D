#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>

#include <string>
#include <array>

#include "Vector.h"
#include "Colour.h"

namespace Run {
	class Window
	{
	public:
		static Window& get();
		void setSize(int width, int height);
		void setWidth(int width);
		void setHeight(int height);
		void setTitle(std::string& title);
		void setTitle(std::string&& title);

		void getInputEvents();

		void destroy();

		bool shouldClose();

		bool isKeyDown(uint32_t key);
		bool isKeyUp(uint32_t key);
		bool isKeyJustDown(uint32_t key);
		bool isKeyJustUp(uint32_t key);

		Math::Vector2 getMousePos();
		void setMousePos(Math::Vector2 pos);
		

		void setColour(Colour colour);
		Colour getColour();

		GLFWwindow*& getGLFWwindow();

		Window(Window&) = delete;
	private:
		Window();

		static Window* m_instance;

		GLFWwindow* m_window = nullptr;
		int m_width = 1280, m_height = 720;
		std::string m_title = "";

		Math::Vector4 m_colour{0.0f, 0.0f, 0.0f, 0.0f};

		struct {
			std::array<bool, 1> keys;
			Math::Vector2 mousePos{0.0f, 0.0f};
		} m_input;
	};
}

