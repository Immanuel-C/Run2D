#include "VkPhysicalDevice.h"

namespace Run {
    namespace Vk {
        PhysicalDevice::PhysicalDevice(Instance& instance, Surface& surface) : m_instance{&instance}, m_surface{&surface} {
            I_DEBUG_LOG_INFO("Getting physical device... | RunEngine");

            uint32_t physicalDeviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
            
            I_ASSERT_FATAL_ERROR(!physicalDeviceCount, "Failed to find a physical device that supports Vulkan!");

            std::vector<VkPhysicalDevice> physicalDevices{physicalDeviceCount};
            vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

            for (auto& device : physicalDevices) {
                if (physicalDeviceSuitable(device)) {
                    m_device = device;
                    break;
                }
            }


            I_ASSERT_FATAL_ERROR(!m_device, "Failed to find a suitable physical device!");

            VkPhysicalDeviceProperties properties{};
            vkGetPhysicalDeviceProperties(m_device, &properties);


            const char* sDeviceType;
            switch (properties.deviceType) {
                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    sDeviceType = "Other";
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    sDeviceType = "Integrated GPU";
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    sDeviceType = "Discrete GPU";
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    sDeviceType = "Virtual GPU";
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    sDeviceType = "CPU";
                    break;
                default:
                    sDeviceType = "Unkown";
                    break;
            };

            I_DEBUG_LOG_INFO("%s (%s) | RunEngine", properties.deviceName, sDeviceType); 
        }

        VkPhysicalDevice& PhysicalDevice::getVkPhysicalDevice() { return m_device; }
        QueueFamilyIndices PhysicalDevice::getQueueFamilyIndices() { return m_queueIndices; }
        SwapChainDetails PhysicalDevice::getSwapChainDetails() { return m_swapChainDetails; }

        void PhysicalDevice::findQueueFamilies(VkPhysicalDevice& device) {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            for (uint32_t i = 0; i < queueFamilyCount; i++) {
                // Checks if this queue family is a graphics queue family
                // this a a bit mask instead of a regular variable because the graphics family also could
                // be the compute family, etc.
                if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    m_queueIndices.graphicsFamilyIdx = i;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *m_surface, &presentSupport);

                if (presentSupport)
                    m_queueIndices.presentFamilyIdx = i;

                // If all queueFamily indices are found then exit loop
                if (m_queueIndices.isValid()) 
                    break;
            }
        }

        
        void PhysicalDevice::querySwapChainSupport(VkPhysicalDevice& device) {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *m_surface, &m_swapChainDetails.capabilities);

            uint32_t formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, *m_surface, &formatCount, nullptr);

            if (formatCount) {
                m_swapChainDetails.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, *m_surface, &formatCount, m_swapChainDetails.formats.data());
            }

            uint32_t presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, *m_surface, &presentModeCount, nullptr);

            if (presentModeCount) {
                m_swapChainDetails.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, *m_surface, &presentModeCount, m_swapChainDetails.presentModes.data());
            }
        }

        bool PhysicalDevice::checkPhysicalDeviceExtensionSupport(VkPhysicalDevice& device, std::vector<const char*>& extensions) {
            uint32_t extensionCount = 0;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions{extensionCount};
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            uint32_t supportedExtensions = 0;

            for (const auto& extension : extensions) {
                for (const auto& availableExtension : availableExtensions) {
                    if (strcmp(extension, availableExtension.extensionName) == NULL) {
                        supportedExtensions++;
                        break;
                    }
                }
            }

            return supportedExtensions == extensions.size();
        }

        bool PhysicalDevice::physicalDeviceSuitable(VkPhysicalDevice& device) {            
            VkPhysicalDeviceProperties properties{};
            vkGetPhysicalDeviceProperties(device, &properties);
            VkPhysicalDeviceFeatures features{};
            vkGetPhysicalDeviceFeatures(device, &features);
            
            std::vector<const char*> extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };


            bool extensionsSupported = checkPhysicalDeviceExtensionSupport(device, extensions);

            querySwapChainSupport(device);


            // could check features and properties

            findQueueFamilies(device); 

            bool swapChainAdquate = false;

            if (extensionsSupported) 
                swapChainAdquate = !m_swapChainDetails.formats.empty() && !m_swapChainDetails.presentModes.empty(); 

            
            return m_queueIndices.isValid() && swapChainAdquate;
        }

    }
}