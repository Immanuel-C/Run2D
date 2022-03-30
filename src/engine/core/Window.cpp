#include "Window.h"

namespace Run {
    Window* Window::m_instance;

    Window& Window::get()
    {
        if (!m_instance)
            m_instance = new Window;

        return *m_instance;
    }

    void Window::setSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        glfwSetWindowSize(m_window, width, height);
    }

    void Window::setWidth(int width)
    {
        setSize(width, m_height);
    }

    void Window::setHeight(int height)
    {
        setSize(m_width, height);
    }

    void Window::setTitle(std::string& title)
    {
        m_title = title;
        glfwSetWindowTitle(m_window, m_title.c_str());
    }

    void Window::setTitle(std::string&& title)
    {
        m_title = std::move(title);
        glfwSetWindowTitle(m_window, m_title.c_str());
    }

    void Window::getInputEvents()
    {
        glfwPollEvents();
    }

    void Window::destroy()
    {
        I_DEBUG_LOG_INFO("Destroying GLFW window... | RunEngine");
        glfwDestroyWindow(m_window);
        I_DEBUG_LOG_INFO("Terminating GLFW... | RunEngine");
        glfwTerminate();

        delete this;
    }

    bool Window::shouldClose() { return glfwWindowShouldClose(m_window); }

    void Window::setColour(Math::Colour colour) { m_colour = colour; }

    Math::Vector4 Window::getColour() { return m_colour; }

    GLFWwindow*& Window::getGLFWwindow() { return m_window; }

    Window::Window()
    {
        I_DEBUG_LOG_INFO("Initing GLFW... | RunEngine");
        I_ASSERT_FATAL_ERROR(!glfwInit(), "Failed to init GLFW!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        I_DEBUG_LOG_INFO("Checking if the required GLFW extensions are supported... | RunEngine");
        I_ASSERT_FATAL_ERROR(!glfwVulkanSupported(), "The required GLFW extensions are not supported!");

        I_DEBUG_LOG_INFO("Creating GLFW window... | RunEngine");
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        I_ASSERT_FATAL_ERROR(!m_window, "Could not create a GLFW window!");
    }
}
