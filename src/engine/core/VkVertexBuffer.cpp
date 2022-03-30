#include "VkVertexBuffer.h"

namespace Run {
	namespace Vk {
		std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescs{};

			// position
			attributeDescs[0].binding = 0;
			attributeDescs[0].location = 0;
			// 2 floats
			attributeDescs[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescs[0].offset = offsetof(Vertex, position);

			// colour
			attributeDescs[1].binding = 0;
			attributeDescs[1].location = 1;
			// 3 floats
			attributeDescs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescs[1].offset = offsetof(Vertex, colour);

			return attributeDescs;
		}
		VkVertexInputBindingDescription Vertex::getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDesc{};
			bindingDesc.binding = 0;
			bindingDesc.stride = sizeof(Vertex);
			// VK_VERTEX_INPUT_RATE_INSTANCE changes vertex index every instance
			// VK_VERTEX_INPUT_RATE_VERTEX changes vertex for every vertex
			bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDesc;
		}

		VertexBuffer::VertexBuffer(Vertex* vertices, size_t verticesSize)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			// Size in bytes
			bufferInfo.size = sizeof(Vertex) * verticesSize;
			bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			I_DEBUG_LOG_INFO("Creating vertex buffer... | Run Engine");

			VK_CHECK(vkCreateBuffer(m_context.device, &bufferInfo, nullptr, &m_buffer));
		}

		void VertexBuffer::destroy()
		{
			I_DEBUG_LOG_INFO("Destroying vertex buffer... | Run Engine");
			vkDestroyBuffer(m_context.device, m_buffer, nullptr);
		}
	}
}