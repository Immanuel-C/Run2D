#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>

#include "VkBufferUtils.h"
#include "Vector.h"
#include "debug.h"
#include "VkContext.h"

#include <array>


namespace Run {
	namespace Vk {
		struct Vertex {
			Math::Vector2 position;
			Math::Vector3 colour;

			static VkVertexInputBindingDescription getBindingDescription();

			static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
		};

		class VertexBuffer
		{
		public:
			VertexBuffer(Vertex* vertices, size_t verticesSize);
			VertexBuffer() {}

			VkBuffer& getVkBuffer();

			size_t getVerticesSize();

			void destroy();
		private:

			size_t m_verticesSize = 0;

			static VkCommandPool m_commandPool;
			static int64_t m_bufferCount;

			Context& m_context = Context::get();
			VkBuffer m_buffer = nullptr;
			VkDeviceMemory m_bufferMemory = nullptr;
		};
	}
}
