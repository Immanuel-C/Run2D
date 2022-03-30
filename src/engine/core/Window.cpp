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
        m_size.x = width;
        m_size.y = height;
        glfwSetWindowSize(m_window, m_size.x, m_size.y);
    }

    void Window::setWidth(int width)
    {
        setSize(width, m_size.y);
    }

    void Window::setHeight(int height)
    {
        setSize(m_size.x, height);
    }

    void Window::setPosition(int x, int y)
    {
        m_position.x = x;
        m_position.y = y;
        glfwSetWindowPos(m_window, m_position.x, m_position.y);
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

    void Window::center()
    {
        const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        m_position.x = (vidMode->width - m_size.x) / 2;
        m_position.y = (vidMode->height - m_size.y) / 2;

        setPosition(m_position.x, m_position.y);
    }

    void Window::getInputEvents()
    {
        glfwPollEvents();
    }

    void Window::destroy()
    {
        if (m_cursor) {
            I_DEBUG_LOG_INFO("Destroying GLFW cursor... | RunEngine");
            glfwDestroyCursor(m_cursor);
        }

        I_DEBUG_LOG_INFO("Destroying GLFW window... | RunEngine");
        glfwDestroyWindow(m_window);
        I_DEBUG_LOG_INFO("Terminating GLFW... | RunEngine");
        glfwTerminate();

        delete this;
    }

    bool Window::shouldClose() { return glfwWindowShouldClose(m_window); }

    bool Window::isFullscreen() { return m_fullscreen; }

    bool Window::isKeyDown(uint32_t key)
    {
        if (key > MAX_KEYS)
            return false;

        return m_input.keys[key];
    }

    bool Window::isKeyUp(uint32_t key)
    {
        return !isKeyDown(key);
    }

    bool Window::isKeyJustDown(uint32_t key)
    {
        if (key > MAX_KEYS)
            return false;

        return m_input.keysJustPressed[key];
    }

    bool Window::isKeyJustUp(uint32_t key)
    {
        return !isKeyJustDown(key);
    }

    Math::Vector2 Window::getMousePosition()
    {
        return m_input.mousePos;
    }

    void Window::setMousePosition(int x, int y)
    {
        m_input.mousePos.x = x;
        m_input.mousePos.y = y;

        glfwSetCursorPos(m_window, m_input.mousePos.x, m_input.mousePos.y);
    }

    void Window::setCursor(int cursor)
    {
        m_cursor = glfwCreateStandardCursor(cursor);
        glfwSetCursor(m_window, m_cursor);
    }

    void Window::setCursor(Cursor cursor, int hotX, int hotY)
    {
        GLFWimage image{};
        image.width = cursor.width;
        image.height = cursor.height;
        image.pixels = cursor.data;

        m_cursor = glfwCreateCursor(&image, hotX, hotY);
        glfwSetCursor(m_window, m_cursor);
    }

    void Window::defaultCursor()
    {
        glfwSetCursor(m_window, nullptr);
        if (m_cursor) {
            glfwDestroyCursor(m_cursor);
            m_cursor = nullptr;
        }
    }

    void Window::setColour(Colour colour) { m_colour = colour; }

    Colour Window::getColour() { return m_colour; }

    void Window::setFullscreen(bool fullscreen)
    {
        if (m_fullscreen == fullscreen)
            return;

        m_fullscreen = fullscreen;

        const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());


        glfwSetWindowMonitor
        (
            m_window, 
            fullscreen ? glfwGetPrimaryMonitor() : nullptr, 
            m_position.x, m_position.y,
            m_size.x, m_size.y,
            GLFW_DONT_CARE
        );

        if (!m_fullscreen)
            center();
    }

    Math::Vector2 Window::getSize()
    {
        return m_size;
    }

    Math::Vector2 Window::getPosition()
    {
        return m_position;
    }

    bool Window::isFramebufferResized() { return m_framebufferResized; }

    void Window::setFramebufferResizedFalse() { m_framebufferResized = false; }

    GLFWwindow*& Window::getGLFWwindow() { return m_window; }

    Window::Window()
    {
        I_DEBUG_LOG_INFO("Initing GLFW... | RunEngine");
        I_ASSERT_FATAL_ERROR(!glfwInit(), "Failed to init GLFW!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        I_DEBUG_LOG_INFO("Checking if the required GLFW extensions are supported... | RunEngine");
        I_ASSERT_FATAL_ERROR(!glfwVulkanSupported(), "The required GLFW extensions are not supported!");

        I_DEBUG_LOG_INFO("Creating GLFW window... | RunEngine");
        m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);

        I_ASSERT_FATAL_ERROR(!m_window, "Could not create a GLFW window!");

        for (uint32_t i = 0; i < MAX_KEYS; i++) {
            m_input.keys[i] = false;
            m_input.keysJustPressed[i] = false;
        }

        glfwSetWindowUserPointer(m_window, this);
        glfwSetWindowPosCallback(m_window, windowMovedCallback);
        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetCursorPosCallback(m_window, cursorPositionCallback);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    }

    void Window::windowMovedCallback(GLFWwindow* window, int xpos, int ypos)
    {
        Window* win = (Window*)(glfwGetWindowUserPointer(window));

        win->m_position.x = xpos;
        win->m_position.y = ypos;
    }

    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key > MAX_KEYS)
            return;

        Window* win = (Window*)(glfwGetWindowUserPointer(window));

        win->m_input.keys[key] = action != GLFW_RELEASE;
        win->m_input.keysJustPressed[key] = action == GLFW_PRESS;
    }

    void Window::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Window* win = (Window*)(glfwGetWindowUserPointer(window));

        win->m_input.mousePos = { (float)xpos, (float)ypos };
    }
    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        Window* win = (Window*)(glfwGetWindowUserPointer(window));

        win->m_framebufferResized = true;
        win->m_size = { (float)width, (float)height };
    }
}
