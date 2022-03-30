#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>
#include <vector>
#include <array>

#include "debug.h"
#include "VkInstance.h"
#include "VkSurface.h"
#include "VkStructs.h"

namespace Run {
    namespace Vk {
        class PhysicalDevice {
        public:
            PhysicalDevice(Instance& instance, Surface& surface);
            PhysicalDevice() {}

            VkPhysicalDevice& getVkPhysicalDevice();
            QueueFamilyIndices getQueueFamilyIndices();
            SwapChainDetails getSwapChainDetails();

            operator VkPhysicalDevice&() { return m_device; }
        private:
            bool physicalDeviceSuitable(VkPhysicalDevice& device);
            void findQueueFamilies(VkPhysicalDevice& device);
            bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice& device, std::vector<const char*>& extensions);
            void querySwapChainSupport(VkPhysicalDevice& device);

            VkPhysicalDevice m_device{};

            QueueFamilyIndices m_queueIndices{};
            SwapChainDetails m_swapChainDetails{};

            Surface* m_surface{};
            Instance* m_instance{};
        };
    }
}