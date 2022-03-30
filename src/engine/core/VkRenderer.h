#pragma once

#include "VkSwapChain.h"
#include "VkGraphicsPipeline.h"
#include "Window.h"
#include "Ref.h"

namespace Run {
	namespace Vk {
		class Renderer
		{
		public:
			Renderer(GraphicsPipeline& graphicsPipeline);

			void draw();

			void destroy();
		private:
			void createSync();
			void createCommandPool();
			void allocateCommandBuffer();
			void recordCommandBuffer(uint32_t imageIndex);

			void recreate();

			uint32_t m_currentFrame = 0;

			VkCommandPool m_commandPool;
			std::vector<VkCommandBuffer> m_commandBuffers;

			SwapChain m_swapChain{};
			GraphicsPipeline& m_graphicsPipeline;
			Window& m_window = Window::get();
			Context& m_context = Context::get();

			struct {
				std::vector<VkSemaphore> imageAvailableSemaphores;
				std::vector<VkSemaphore> renderFinishedSemaphores;
				std::vector<VkFence> inFlightFences;
			} m_sync;
		};
	}
}

