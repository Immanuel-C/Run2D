#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>
#include <vector>
#include <array>
#include "VkInstance.h"
#include "debug.h"
#include "DynamicExport.h"

namespace Run {
    namespace Vk {
        class RUN_API DebugMessenger {
        public:
            DebugMessenger(Instance& instance);
            DebugMessenger() {}

            void destroy();

            VkDebugUtilsMessengerEXT& getVkDebugMessenger();

            operator VkDebugUtilsMessengerEXT&() { return m_debugMessenger; }
        private:
            Instance* m_instance{};
            VkDebugUtilsMessengerEXT m_debugMessenger{};

            static VkBool32 VKAPI_PTR debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT  messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        };
    }
}