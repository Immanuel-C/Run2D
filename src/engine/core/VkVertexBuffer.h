#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>

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

			VkBuffer& getVkBuffer();

			void destroy();
		private:
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

			Context& m_context = Context::get();
			VkBuffer m_buffer;
			VkDeviceMemory m_bufferMemory;
		};
	}
}
