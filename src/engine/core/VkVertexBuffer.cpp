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

			VkMemoryRequirements memRequirements{};
			vkGetBufferMemoryRequirements(m_context.device, m_buffer, &memRequirements);


			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			VK_CHECK(vkAllocateMemory(m_context.device, &allocInfo, nullptr, &m_bufferMemory));

			// Associate this memory with the buffer
			vkBindBufferMemory(m_context.device, m_buffer, m_bufferMemory, 0);

			void* data;
			// make the data buffer a cpu visible buffer to the physical device memory
			vkMapMemory(m_context.device, m_bufferMemory, 0, bufferInfo.size, 0, &data);
			memcpy(data, vertices, bufferInfo.size);
			// the VK_MEMORY_PROPERTY_HOST_COHERENT_BIT makes sure to copy the data to the physical device buffer
			// the data buffer with un-associate with the physical device buffer
			vkUnmapMemory(m_context.device, m_bufferMemory);
		}

		VkBuffer& VertexBuffer::getVkBuffer() { return m_buffer; }

		void VertexBuffer::destroy()
		{
			I_DEBUG_LOG_INFO("Destroying vertex buffer... | Run Engine");
			vkDestroyBuffer(m_context.device, m_buffer, nullptr);
			vkFreeMemory(m_context.device, m_bufferMemory, nullptr);
		}

		uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			// Get the memory properties (which contains the memory types)
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(m_context.physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				// 1. Gets the index of the memory type. idk what (1 << i) does though
				// 2. Check for the properties flags. the properties flags are useful for checking if a 
				// memory type is visible to host, etc.
				if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			I_ASSERT_ERROR(true, "Failed to find a memory type for vertex buffer!");

			return 0;
		}
	}
}