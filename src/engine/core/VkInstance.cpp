#include "VkInstance.h"


namespace Run {
    namespace Vk {
        bool checkInstanceExtensionsSupport(const char** extensions, size_t extensionSize) {
            I_DEBUG_LOG_INFO("Checking if instance extensions are supported... | RunEngine");
            
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            if (!extensionCount)
                return false;

            std::vector<VkExtensionProperties> extensionsProperties{extensionCount};
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsProperties.data());

            uint32_t supportedExtensionCount = 0;

            for (const auto& extensionProperty : extensionsProperties) {
                for (uint32_t i = 0; i < extensionSize; i++) {
                    if (strcmp(extensionProperty.extensionName, extensions[i]) == NULL) 
                        supportedExtensionCount++;
                }
            }

            if (supportedExtensionCount != extensionSize)
                return false;

            return true;
        }

        bool Instance::checkInstanceLayerSupport(const char** layers, size_t layerSize) {
            I_DEBUG_LOG_INFO("Checking if instance layers are supported... | RunEngine");

            uint32_t layerCount = 0;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            if (!layerCount)
                return false;
            
            std::vector<VkLayerProperties> layerProperties{layerCount};
            vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());

            uint32_t supportedLayerCount = 0;

            for (const auto& layerProperty : layerProperties) {
                for (uint32_t i = 0; i < layerSize; i++) {
                    if (strcmp(layerProperty.layerName, layers[i]) == NULL) 
                        supportedLayerCount++; 
                }
            }

            if (supportedLayerCount != layerSize)
                return false;

            return true;
        }

        Instance::Instance(uint32_t vkVersion) {
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pEngineName = "Run-Vk";
            appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
            appInfo.apiVersion = vkVersion;

            VkInstanceCreateInfo instanceCreateInfo{};
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pApplicationInfo = &appInfo;

            std::vector<const char*> instanceExtensionNames = {
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME
            };

            uint32_t glfwInstanceExtensionCount = 0;
            const char** glfwInstanceExtensionNames = glfwGetRequiredInstanceExtensions(&glfwInstanceExtensionCount);

            for (uint32_t i = 0; i < glfwInstanceExtensionCount; i++) {
                instanceExtensionNames.push_back(glfwInstanceExtensionNames[i]);
            } 

            I_ASSERT_FATAL_ERROR(!checkInstanceExtensionsSupport(instanceExtensionNames.data(), instanceExtensionNames.size()), "Vulkan instance extension(s) not supported!");


            instanceCreateInfo.enabledExtensionCount = instanceExtensionNames.size();
            instanceCreateInfo.ppEnabledExtensionNames = instanceExtensionNames.data();

            std::vector<const char*> instanceLayerNames = {
#if defined(DEBUG) || !defined(NDEBUG)
                "VK_LAYER_KHRONOS_validation",
#endif
#if defined(RUN_VK_LAYER_MONITOR)
                "VK_LAYER_LUNARG_monitor"
#endif
            };

            // instanceLayerNames size will be zero ifndef RUN_VK_LAYER_MONITOR and not in debug mode
            // so we dont need to check for extensions or add extensions to instanceCreateInfo
            if (instanceLayerNames.size()) {
                I_ASSERT_FATAL_ERROR(!checkInstanceLayerSupport(instanceLayerNames.data(), instanceLayerNames.size()), "Vulkan instance layer(s) not supported!");

                instanceCreateInfo.enabledLayerCount = instanceLayerNames.size();
                instanceCreateInfo.ppEnabledLayerNames = instanceLayerNames.data();
            }

            I_DEBUG_LOG_INFO("Creating instance... | RunEngine");
            VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
        }

        void Instance::destroy() {
            I_DEBUG_LOG_INFO("Destroying instance... | RunEngine");
            
            vkDestroyInstance(m_instance, nullptr);
        }

        VkInstance& Instance::getVkInstance() {
            return m_instance;
        }
    }
}