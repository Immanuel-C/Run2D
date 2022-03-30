#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>
#include <vector>
#include <array>
#include <algorithm>

#include "VkStructs.h"
#include "debug.h"

#include "VkInstance.h"
#include "VkSurface.h"
#include "VkDevice.h"
#include "VkPhysicalDevice.h"
#include "VkGraphicsPipeline.h"

namespace Run {
    namespace Vk {
        class SwapChain
        {
        public:
            SwapChain(Device& device, PhysicalDevice& physicalDevice, GraphicsPipeline& graphicsPipeline, Surface& surface, GLFWwindow*& window, VkSurfaceFormatKHR format, VkPresentModeKHR presentMode, VkExtent2D extent);
            SwapChain() {}

            VkSwapchainKHR& getVkSwapChain();

            const std::vector<VkImage> getSwapChainImages();
            const std::vector<VkImageView> getSwapChainImageViews();
            const std::vector<VkFramebuffer>& getSwapChainFramebuffers();
            VkExtent2D getSwapChainExtent();
            VkFormat getSwapChainImageFormat();

            void destroy();

            operator VkSwapchainKHR() { return m_swapChain; }
        private:


            void createSwapChain(Device& device, PhysicalDevice& physicalDevice, Surface& surface);
            void createImageViews(Device& device);
            void createFramebuffers(Device& device, Run::Vk::GraphicsPipeline& graphicsPipeline);


            VkSwapchainKHR m_swapChain;
            std::vector<VkImage> m_swapChainImages;
            std::vector<VkImageView> m_swapChainImageViews;
            std::vector<VkFramebuffer> m_swapChainFrameBuffers;
            VkSurfaceFormatKHR m_swapChainImageFormat{};
            VkExtent2D m_swapChainExtent{};
            VkPresentModeKHR m_swapChainPresentMode{};
            GLFWwindow* m_window;
            Device* m_device;
        };
    }
}

