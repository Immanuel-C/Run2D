#include "VkDevice.h"


namespace Run {
    namespace Vk {
        Device::Device(PhysicalDevice physicalDevice) {
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            // If the graphics family idx and the present family idx are the same one of the values will get
            // discarded from the set
            std::set<int> uniqueQueueFamiliesIdxs = {physicalDevice.getQueueFamilyIndices().graphicsFamilyIdx, physicalDevice.getQueueFamilyIndices().presentFamilyIdx};

            float queuePriority = 1.0f;
            
            for (const int32_t& queueFamilyIdx : uniqueQueueFamiliesIdxs) {
                VkDeviceQueueCreateInfo queueCreateInfo{};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = queueFamilyIdx;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = &queuePriority;
                
                queueCreateInfos.push_back(queueCreateInfo);
            }

            VkPhysicalDeviceFeatures features{};
            vkGetPhysicalDeviceFeatures(physicalDevice, &features);

            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.pQueueCreateInfos = queueCreateInfos.data();
            createInfo.queueCreateInfoCount = queueCreateInfos.size();
            createInfo.pEnabledFeatures = &features;

            std::array<const char*, 1> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };


            createInfo.enabledExtensionCount = deviceExtensions.size();
            createInfo.ppEnabledExtensionNames = deviceExtensions.data();

            VK_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device));

            vkGetDeviceQueue(m_device, physicalDevice.getQueueFamilyIndices().graphicsFamilyIdx, 0, &m_queues.graphics);
            vkGetDeviceQueue(m_device, physicalDevice.getQueueFamilyIndices().presentFamilyIdx, 0, &m_queues.present);
        }


        VkDevice& Device::getVkDevice() { return m_device; }
        Queues& Device::getQueues() { return m_queues; }

        void Device::destroy()
        {

            I_DEBUG_LOG_INFO("Destroying device... | RunEngine");
            vkDestroyDevice(m_device, nullptr);
        }
    }
}