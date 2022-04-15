#pragma once

#include "VkSwapChain.h"
#include "VkGraphicsPipeline.h"
#include "Window.h"

#include "VkVertexBuffer.h"
#include "VkIndexBuffer.h"
#include "DynamicExport.h"


namespace Run {
	namespace Vk {

		class RUN_API Renderer
		{
		public:
			Renderer(GraphicsPipeline& graphicsPipeline);
			Renderer() : m_graphicsPipeline{ GraphicsPipeline{} } {};

			void draw(VertexBuffer* vertexBuffers, size_t vertexBufferSize, IndexBuffer* indexBuffer, size_t indexBufferSize);

			void destroy();

			void operator=(Renderer& renderer);
			//void operator=(Renderer&& renderer) noexcept;
			
		private:
			void createSync();
			void createCommandPool();
			void allocateCommandBuffer();
			void recordCommandBuffer(uint32_t imageIndex, VertexBuffer* vertexBuffers, size_t vertexBufferSize, IndexBuffer* indexBuffer, size_t indexBufferSize);

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

