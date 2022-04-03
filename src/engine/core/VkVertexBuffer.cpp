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

		VkCommandPool VertexBuffer::m_commandPool = nullptr;
		int64_t VertexBuffer::m_bufferCount = 0;

		VertexBuffer::VertexBuffer(Vertex* vertices, size_t verticesSize) : m_verticesSize { verticesSize }
		{
			m_bufferCount++;

			I_DEBUG_LOG_INFO("Creating vertex buffer... | Run Engine");
			VkDeviceSize bufferSize = sizeof(Vertex) * verticesSize;

			if (!m_commandPool) {
				VkCommandPoolCreateInfo poolInfo{};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = m_context.queueIndices.transferFamilyIdx;

				VK_CHECK(vkCreateCommandPool(m_context.device, &poolInfo, nullptr, &m_commandPool));
			}

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(m_context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			
			void* data;
			// make the data buffer a cpu visible buffer to the physical device memory
			VK_CHECK(vkMapMemory(m_context.device, stagingBufferMemory, 0, bufferSize, 0, &data));
			memcpy(data, vertices, bufferSize);
			// the VK_MEMORY_PROPERTY_HOST_COHERENT_BIT makes sure to copy the data to the physical device buffer
			// the data buffer with un-associate with the physical device buffer
			vkUnmapMemory(m_context.device, stagingBufferMemory);

			createBuffer(m_context, bufferSize,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | 
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory);


			bufcpy(m_context, m_commandPool, m_buffer, stagingBuffer, bufferSize);


			vkDestroyBuffer(m_context.device, stagingBuffer, nullptr);
			vkFreeMemory(m_context.device, stagingBufferMemory, nullptr);
		}

		VkBuffer& VertexBuffer::getVkBuffer() { return m_buffer; }

		size_t VertexBuffer::getVerticesSize()
		{
			return m_verticesSize;
		}

		void VertexBuffer::destroy()
		{
			vkQueueWaitIdle(m_context.device.getQueues().transfer);

			I_DEBUG_LOG_INFO("Destroying vertex buffer... | Run Engine");
			vkDestroyBuffer(m_context.device, m_buffer, nullptr);
			vkFreeMemory(m_context.device, m_bufferMemory, nullptr);

			m_bufferCount--;

			if (m_bufferCount == 0) {
				vkDestroyCommandPool(m_context.device, m_commandPool, nullptr);
			}
		}
	}
}