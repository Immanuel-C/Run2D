#include "VkIndexBuffer.h"

namespace Run {
	namespace Vk {
		VkCommandPool IndexBuffer::m_commandPool = nullptr;
		int64_t IndexBuffer::m_bufferCount = 0;

		IndexBuffer::IndexBuffer(uint32_t* indices, size_t indicesSize) : m_indicesSize{ indicesSize }
		{
			m_bufferCount++;

			I_DEBUG_LOG_INFO("Creating index buffer... | Run Engine");
			VkDeviceSize bufferSize = sizeof(uint32_t) * indicesSize;

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
			memcpy(data, indices, bufferSize);
			// the VK_MEMORY_PROPERTY_HOST_COHERENT_BIT makes sure to copy the data to the physical device buffer
			// the data buffer with un-associate with the physical device buffer
			vkUnmapMemory(m_context.device, stagingBufferMemory);

			createBuffer(m_context, bufferSize,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT |
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory);


			bufcpy(m_context, m_commandPool, m_buffer, stagingBuffer, bufferSize);


			vkDestroyBuffer(m_context.device, stagingBuffer, nullptr);
			vkFreeMemory(m_context.device, stagingBufferMemory, nullptr);
		}

		VkBuffer& IndexBuffer::getVkBuffer() { return m_buffer; }

		size_t IndexBuffer::getIndicesSize() { return  m_indicesSize; }

		void IndexBuffer::destroy()
		{
			vkQueueWaitIdle(m_context.device.getQueues().transfer);

			I_DEBUG_LOG_INFO("Destroying index buffer... | Run Engine");
			vkDestroyBuffer(m_context.device, m_buffer, nullptr);
			vkFreeMemory(m_context.device, m_bufferMemory, nullptr);

			m_bufferCount--;
			if (m_bufferCount == 0) 
				vkDestroyCommandPool(m_context.device, m_commandPool, nullptr);
		}
	}
}