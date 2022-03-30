#include "VkSwapChain.h"

namespace Run {
    namespace Vk {
        SwapChain::SwapChain(Device& device, PhysicalDevice& physicalDevice, GraphicsPipeline& graphicsPipeline, Surface& surface, GLFWwindow*& window, VkSurfaceFormatKHR format, VkPresentModeKHR presentMode, VkExtent2D extent, VkSwapchainKHR oldSwapChain)
            : m_window{ window }, 
              m_device{ &device },
              m_swapChainExtent{ extent },
              m_swapChainImageFormat{ format },
              m_swapChainPresentMode{ presentMode }
        {
            createSwapChain(device, physicalDevice, surface, oldSwapChain);
            createImageViews(device);
            createFramebuffers(device, graphicsPipeline);
        }

        void SwapChain::createSwapChain(Device& device, PhysicalDevice& physicalDevice, Surface& surface, VkSwapchainKHR oldSwapChain)
        {
            I_DEBUG_LOG_INFO("Creating swap chain... | RunEngine");

            SwapChainDetails swapChainDetails = physicalDevice.getSwapChainDetails();


            uint32_t imageCount = swapChainDetails.capabilities.minImageCount + 1;

            // If max image count is zero that means there is no max count
            imageCount = imageCount > swapChainDetails.capabilities.maxImageCount
                && swapChainDetails.capabilities.maxImageCount > 0
                ? swapChainDetails.capabilities.maxImageCount : imageCount;

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = surface;
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = m_swapChainImageFormat.format;
            createInfo.imageColorSpace = m_swapChainImageFormat.colorSpace;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            createInfo.imageArrayLayers = 1;
            createInfo.imageExtent = m_swapChainExtent;

            std::array<int, 2> queueFamilyIndices = {
                physicalDevice.getQueueFamilyIndices().graphicsFamilyIdx,
                physicalDevice.getQueueFamilyIndices().presentFamilyIdx
            };

            // How to handle images between different queues
            if (physicalDevice.getQueueFamilyIndices().graphicsFamilyIdx != physicalDevice.getQueueFamilyIndices().presentFamilyIdx) {
                // Images can be used between multiple queues
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
                createInfo.pQueueFamilyIndices = (uint32_t*)queueFamilyIndices.data();
            }
            else
                // if the graphics family and the present family are the same
                // then set the mode to exclusive 
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

            createInfo.preTransform = swapChainDetails.capabilities.currentTransform;
            createInfo.presentMode = m_swapChainPresentMode;
            // If another window is in front of our window then dont render the blocked section
            createInfo.clipped = VK_TRUE;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.oldSwapchain = oldSwapChain;

            VK_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain));

            I_DEBUG_LOG_INFO("Getting swap chain images... | RunEngine");

            VK_CHECK(vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, nullptr));
            m_swapChainImages.resize(imageCount);
            VK_CHECK(vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, m_swapChainImages.data()));

        }

        void SwapChain::createImageViews(Device& device) {
            I_DEBUG_LOG_INFO("Creating swap chain image views... | RunEngine");

            m_swapChainImageViews.resize(m_swapChainImages.size());

            for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
                VkImageViewCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                createInfo.image = m_swapChainImages[i];
                createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                createInfo.format = m_swapChainImageFormat.format;
                // For swizzling the colour (e.g. colour.rga)
                createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                // For 3D images
                createInfo.subresourceRange.layerCount = 1;
                createInfo.subresourceRange.levelCount = 1;

                VK_CHECK(vkCreateImageView(device, &createInfo, nullptr, &m_swapChainImageViews[i]));
            }
        }

        void SwapChain::createFramebuffers(Device& device, GraphicsPipeline& graphicsPipeline)
        {
            I_DEBUG_LOG_INFO("Creating swap chain framebuffers... | RunEngine");

            m_swapChainFrameBuffers.resize(m_swapChainImageViews.size());


            for (uint32_t i = 0; i < m_swapChainImageViews.size(); i++) {

                std::array<VkImageView, 1> attachments = {
                    m_swapChainImageViews[i]
                };

                VkFramebufferCreateInfo framebufferInfo{};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = graphicsPipeline.getVkRenderPass();
                framebufferInfo.attachmentCount = attachments.size();
                framebufferInfo.pAttachments = attachments.data();
                framebufferInfo.width = m_swapChainExtent.width;
                framebufferInfo.height = m_swapChainExtent.height;
                framebufferInfo.layers = 1;


                VK_CHECK(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_swapChainFrameBuffers[i]));
            }
        }


        VkSwapchainKHR& SwapChain::getVkSwapChain() { return m_swapChain;  }

        const std::vector<VkImage> SwapChain::getSwapChainImages() { return m_swapChainImages; }

        const std::vector<VkImageView> SwapChain::getSwapChainImageViews() { return m_swapChainImageViews; }

        const std::vector<VkFramebuffer>& SwapChain::getSwapChainFramebuffers() { return m_swapChainFrameBuffers; }

        VkExtent2D SwapChain::getSwapChainExtent() { return m_swapChainExtent; }

        VkFormat SwapChain::getSwapChainImageFormat() { return m_swapChainImageFormat.format; }

        void SwapChain::destroy(int flags)
        {
            I_DEBUG_LOG_INFO("Destroying swap chain framebuffers... | RunEngine");
            for (const auto& framebuffer : m_swapChainFrameBuffers) {
                vkDestroyFramebuffer(*m_device, framebuffer, nullptr);
            }

            I_DEBUG_LOG_INFO("Destroying swap chain image views... | RunEngine");
            for (const auto& imageView : m_swapChainImageViews) {
                vkDestroyImageView(*m_device, imageView, nullptr);
            }

            if (flags & SWAPCHAIN_DONT_DESTROY_VK_SWAPCHAIN)
                return;

            I_DEBUG_LOG_INFO("Destroying swap chain... | RunEngine");
            vkDestroySwapchainKHR(*m_device, m_swapChain, nullptr);
        }
    }
}