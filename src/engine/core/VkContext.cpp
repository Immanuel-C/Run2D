#include "VkContext.h"

#include <iostream>

namespace Run {
    namespace Vk {
        Context* Context::m_contextInstance;
        
        Context& Context::get() {
            if (!m_contextInstance)
                m_contextInstance = new Context;

            return *m_contextInstance; 
        }

        std::mutex initMutex;

        void initAsync(std::shared_ptr<DebugMessenger>& debugMessenger, Instance& instance) {
            std::lock_guard<std::mutex> lock{ initMutex };
            debugMessenger = std::make_shared<DebugMessenger>(instance);
        }

        Context::Context()
        {

            m_window = Run::Window::get().getGLFWwindow();

            instance = Instance{ VK_VERSION_1_2 };

#if !defined(NDEBUG) || defined(DEBUG)
            m_initFuture = std::async(std::launch::async, initAsync, std::ref(debugMessenger), std::ref(instance));
#endif
            surface = Surface{ instance, m_window };
            physicalDevice = PhysicalDevice{ instance, surface };

            queueIndices = physicalDevice.getQueueFamilyIndices();

            device = Device{ physicalDevice };
        }


        VkPresentModeKHR Context::chooseBestSwapChainPresentMode(std::vector<VkPresentModeKHR>& presentModes)
        {
            for (auto& presentMode : presentModes) {
                if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                    return presentMode;
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D Context::chooseSwapChainExtent(VkSurfaceCapabilitiesKHR& capabilities)
        {
            #if defined(max)
                #undef max
            #endif

            // If the framebuffer size is in between the minImageExtent and the maxImageExtent
            // then we can just return the currentExtent
            // I think this is how it works
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            }
            else {
                int width, height;
                glfwGetFramebufferSize(m_window, &width, &height);

                VkExtent2D actualExtent = {
                    (uint32_t)width,
                    (uint32_t)height
                };

                actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                return actualExtent;
            }
        }

        VkSurfaceFormatKHR Context::chooseBestSwapChainFormat(std::vector<VkSurfaceFormatKHR>& formats) {
            for (auto& format : formats) {
                if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return format;
                }
            }

            // If the preffered format is unavailable then just return the first format
            return formats[0];
        }

        void Context::destroy() {
            I_DEBUG_LOG_INFO("Destroying Vulkan Context...");

            device.destroy();

            surface.destroy();

            // If NDEBUG then debugMessenger will be nullptr
            if (debugMessenger.get()) 
                debugMessenger->destroy();

            instance.destroy();

            delete this;
        }

    }
}