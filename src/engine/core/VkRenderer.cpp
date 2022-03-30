#include "VkRenderer.h"


namespace Run {
	namespace Vk {
        // Allows us to draw x frames at the same time
        static constexpr uint16_t MAX_FRAMES_IN_FLIGHT = 2;

        Renderer::Renderer(GraphicsPipeline& graphicsPipeline) : m_graphicsPipeline{ graphicsPipeline }
        {
            I_DEBUG_LOG_INFO("Creating Vulkan renderer... | RunEngineEngine");

            createSync();
            createCommandPool();
            allocateCommandBuffer();

            {
                SwapChainDetails swapChainDetails = m_context.physicalDevice.getSwapChainDetails();

                VkPresentModeKHR presentMode = m_context.chooseBestSwapChainPresentMode(swapChainDetails.presentModes);
                VkExtent2D extent = m_context.chooseSwapChainExtent(swapChainDetails.capabilities);
                VkSurfaceFormatKHR format = m_context.chooseBestSwapChainFormat(swapChainDetails.formats);

                m_swapChain = SwapChain{ m_context.device, m_context.physicalDevice, graphicsPipeline, m_context.surface, m_window.getGLFWwindow(), format, presentMode, extent, nullptr };
            }
        }


        #if defined(max)
            #undef max
        #endif

        void Renderer::draw(VertexBuffer& buffer)
        {

            VK_CHECK(vkWaitForFences
            (
                m_context.device,
                1,
                &m_sync.inFlightFences[m_currentFrame],
                VK_TRUE,
                std::numeric_limits<uint64_t>::max()
            ));

            uint32_t imageIndex = 0;

            VkResult acquireResult = vkAcquireNextImageKHR
            (
                m_context.device,
                m_swapChain,
                std::numeric_limits<uint64_t>::max(),
                m_sync.imageAvailableSemaphores[m_currentFrame],
                nullptr,
                &imageIndex
            );

            if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
                recreate();
                return;
            }
            else if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
                I_LOG_FATAL_ERROR("Failed to acquire next swap chain image!");
            }

            // Only reset if submitting work
            VK_CHECK(vkResetFences(m_context.device, 1, &m_sync.inFlightFences[m_currentFrame]));


            recordCommandBuffer(imageIndex, buffer);

            VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &m_sync.imageAvailableSemaphores[m_currentFrame];
            submitInfo.pWaitDstStageMask = &waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &m_sync.renderFinishedSemaphores[m_currentFrame];

            VK_CHECK(vkQueueSubmit
            (
                m_context.device.getQueues().graphics,
                1,
                &submitInfo,
                m_sync.inFlightFences[m_currentFrame]
            ));

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &m_sync.renderFinishedSemaphores[m_currentFrame];
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &m_swapChain.getVkSwapChain();
            presentInfo.pImageIndices = &imageIndex;

            VkResult presentResult = vkQueuePresentKHR(m_context.device.getQueues().present, &presentInfo);

            if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR || m_window.isFramebufferResized()) {
                m_window.setFramebufferResizedFalse();
                recreate();
            }
            else if (presentResult != VK_SUCCESS) {
                I_LOG_FATAL_ERROR("Failed to present swap chain image!");
            }

            m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        }

        void Renderer::destroy()
        {
            I_DEBUG_LOG_INFO("Destroying Vulkan renderer... | RunEngine");

            vkQueueWaitIdle(m_context.device.getQueues().graphics);
            vkQueueWaitIdle(m_context.device.getQueues().present);

            for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                vkDestroySemaphore(m_context.device, m_sync.imageAvailableSemaphores[i], nullptr);
                vkDestroySemaphore(m_context.device, m_sync.renderFinishedSemaphores[i], nullptr);
                vkDestroyFence(m_context.device, m_sync.inFlightFences[i], nullptr);
                vkFreeCommandBuffers(m_context.device, m_commandPool, 1, &m_commandBuffers[i]);
            }

            vkDestroyCommandPool(m_context.device, m_commandPool, nullptr);

            m_swapChain.destroy();
        }

        void Renderer::createSync()
        {
            m_sync.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
            m_sync.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
            m_sync.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                VK_CHECK(vkCreateSemaphore(m_context.device, &semaphoreInfo, nullptr, &m_sync.imageAvailableSemaphores[i]));
                VK_CHECK(vkCreateSemaphore(m_context.device, &semaphoreInfo, nullptr, &m_sync.renderFinishedSemaphores[i]));
                VK_CHECK(vkCreateFence(m_context.device, &fenceInfo, nullptr, &m_sync.inFlightFences[i]));
            }

        }

        void Renderer::createCommandPool()
        {
            VkCommandPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            poolInfo.queueFamilyIndex = m_context.queueIndices.graphicsFamilyIdx;

            VK_CHECK(vkCreateCommandPool(m_context.device, &poolInfo, nullptr, &m_commandPool));
        }

        void Renderer::allocateCommandBuffer()
        {
            m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);


            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = m_commandPool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = m_commandBuffers.size();

            VK_CHECK(vkAllocateCommandBuffers(m_context.device, &allocInfo, m_commandBuffers.data()));
        }

        void Renderer::recordCommandBuffer(uint32_t imageIndex, VertexBuffer& buffer)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


            VkClearValue clearColour = {
                m_window.getColour().r,
                m_window.getColour().g,
                m_window.getColour().b,
                m_window.getColour().a
            };

            VkRenderPassBeginInfo renderPassBeginInfo{};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = m_graphicsPipeline.getVkRenderPass();
            renderPassBeginInfo.renderArea.extent = m_swapChain.getSwapChainExtent();
            renderPassBeginInfo.renderArea.offset = { 0, 0 };
            renderPassBeginInfo.framebuffer = m_swapChain.getSwapChainFramebuffers()[imageIndex];
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearColour;

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = m_swapChain.getSwapChainExtent().width;
            viewport.height = m_swapChain.getSwapChainExtent().height;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = { 0, 0 };
            scissor.extent = m_swapChain.getSwapChainExtent();

            {
                VK_CHECK(vkBeginCommandBuffer(m_commandBuffers[m_currentFrame], &beginInfo));

                vkCmdSetViewport(m_commandBuffers[m_currentFrame], 0, 1, &viewport);
                vkCmdSetScissor(m_commandBuffers[m_currentFrame], 0, 1, &scissor);
                {
                    vkCmdBeginRenderPass(m_commandBuffers[m_currentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
                    {
                        vkCmdBindPipeline(m_commandBuffers[m_currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline.getVkPipeline());
                        VkDeviceSize offsets[] = { 0 };
                        vkCmdBindVertexBuffers(m_commandBuffers[m_currentFrame], 0, 1, &buffer.getVkBuffer(), offsets);

                        vkCmdDraw(m_commandBuffers[m_currentFrame], 3, 1, 0, 0);
                    }
                    vkCmdEndRenderPass(m_commandBuffers[m_currentFrame]);
                }

                VK_CHECK(vkEndCommandBuffer(m_commandBuffers[m_currentFrame]));
            }
        }

        void Renderer::recreate()
        {
            int width = 0, height = 0;
            glfwGetFramebufferSize(m_window.getGLFWwindow(), &width, &height);
            while (width == 0 || height == 0) {
                glfwWaitEvents();
                glfwGetFramebufferSize(m_window.getGLFWwindow(), &width, &height);
            }

            vkDeviceWaitIdle(m_context.device);

            m_swapChain.destroy(SWAPCHAIN_DONT_DESTROY_VK_SWAPCHAIN);

            {
                SwapChainDetails swapChainDetails = m_context.physicalDevice.getSwapChainDetails();

                VkPresentModeKHR presentMode = m_context.chooseBestSwapChainPresentMode(swapChainDetails.presentModes);
                VkExtent2D extent = m_context.chooseSwapChainExtent(swapChainDetails.capabilities);
                VkSurfaceFormatKHR format = m_context.chooseBestSwapChainFormat(swapChainDetails.formats);

                m_swapChain = { m_context.device, m_context.physicalDevice, m_graphicsPipeline, m_context.surface, m_window.getGLFWwindow(), format, presentMode, extent, m_swapChain };
            }

        }
	}
}