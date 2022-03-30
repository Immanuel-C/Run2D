#include "VkDebugMessenger.h"

static bool justStarted = true;

namespace Run {
    namespace Vk {
        VkBool32 VKAPI_PTR DebugMessenger::debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT  messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
            I_DEBUG_LOG_WARNING(pCallbackData->pMessage);
            if (justStarted) {
                I_FILE_LOG("VkRun.log", pCallbackData->pMessage, "w");
                justStarted = false;
            }
            else
                I_FILE_LOG("VkRun.log", pCallbackData->pMessage, "a");
                
            return VK_FALSE;
        }

        DebugMessenger::DebugMessenger(Instance& instance) : m_instance{&instance} {
            const auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

            I_DEBUG_ASSERT_ERROR(!vkCreateDebugUtilsMessengerEXT, "Failed to load vkCreateDebugUtilsMessengerEXT function!");

            VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
            debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugMessengerCreateInfo.messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ;
            debugMessengerCreateInfo.messageType = 
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  |
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugMessengerCreateInfo.pfnUserCallback = debugMessengerCallback;
            debugMessengerCreateInfo.pUserData = nullptr;

            I_DEBUG_LOG_INFO("Creating debug messenger... | RunEngine");
            VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &m_debugMessenger));
        }

        void DebugMessenger::destroy() {
            const auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(*m_instance, "vkDestroyDebugUtilsMessengerEXT"));
            I_DEBUG_ASSERT_ERROR(!vkDestroyDebugUtilsMessengerEXT, "Failed to load function vkDestroyDebugUtilsMessengerEXT!");

            I_DEBUG_LOG_INFO("Destroying debug messenger... | RunEngine");
            vkDestroyDebugUtilsMessengerEXT(*m_instance, m_debugMessenger, nullptr);
        }

        VkDebugUtilsMessengerEXT& DebugMessenger::getVkDebugMessenger() {
            return m_debugMessenger;
        }
    }
}