#include "VkSurface.h"


namespace Run {
    namespace Vk {
        Surface::Surface(Instance& instance, GLFWwindow*& window) : m_instance{&instance} {
            I_DEBUG_LOG_INFO("Creating surface... | RunEngine");
            VK_CHECK(glfwCreateWindowSurface(instance, window, nullptr, &m_surface));
        }

        void Surface::destroy() {
            I_DEBUG_LOG_INFO("Destroying surface... | RunEngine");
            vkDestroySurfaceKHR(*m_instance, m_surface, nullptr);
        }
    }
}