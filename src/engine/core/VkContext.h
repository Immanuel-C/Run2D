#pragma once

#include <ILog.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <set>
#include <algorithm>
#include <memory>
#include <future>

#include "debug.h"
#include "VkStructs.h"
#include "VkInstance.h"
#include "VkDebugMessenger.h"
#include "VkSurface.h"
#include "VkPhysicalDevice.h"
#include "VkDevice.h"
#include "Window.h"
#include "Timer.h"
#include "DynamicExport.h"

namespace Run {
    namespace Vk {
        class RUN_API Context
        {
        public:
            static Context& get();


            Instance instance;
            std::shared_ptr<DebugMessenger> debugMessenger;
            PhysicalDevice physicalDevice;
            Device device;
            Surface surface;

            QueueFamilyIndices queueIndices{};

            Context(Context&) = delete;

            VkPresentModeKHR chooseBestSwapChainPresentMode(std::vector<VkPresentModeKHR>& presentModes);
            VkExtent2D chooseSwapChainExtent(VkSurfaceCapabilitiesKHR& capabilities);
            VkSurfaceFormatKHR chooseBestSwapChainFormat(std::vector<VkSurfaceFormatKHR>& formats);

            void destroy();

        private:
            static Context* m_contextInstance;

            GLFWwindow* m_window = nullptr;

            std::future<void> m_initFuture;

            Context();
            ~Context() { }
        };
    }
}
