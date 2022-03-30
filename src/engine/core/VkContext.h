#pragma once

#include <ILog.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <set>
#include <algorithm>
#include <memory>

#include "debug.h"
#include "VkStructs.h"
#include "VkInstance.h"
#include "VkDebugMessenger.h"
#include "VkSurface.h"
#include "VkPhysicalDevice.h"
#include "VkDevice.h"
#include "Window.h"

namespace Run {
    namespace Vk {
        class Context
        {
        public:
            static Context& get();

            Instance instance;
            std::unique_ptr<DebugMessenger> debugMessenger;
            PhysicalDevice physicalDevice;
            Device device;
            Surface surface;

            QueueFamilyIndices queueIndices{};

            Context(Context&) = delete;

            VkPresentModeKHR chooseBestSwapChainPresentMode(std::vector<VkPresentModeKHR>& presentModes);
            VkExtent2D chooseSwapChainExtent(VkSurfaceCapabilitiesKHR& capabilities);
            VkSurfaceFormatKHR chooseBestSwapChainFormat(std::vector<VkSurfaceFormatKHR>& formats);

            void destroy();
            void init(Window& window);

        private:
            static Context* m_contextInstance;

            GLFWwindow* m_window = nullptr;


            Context() { }
            ~Context() { }
        };
    }
}
