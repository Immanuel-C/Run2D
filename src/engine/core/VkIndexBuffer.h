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
		class IndexBuffer
		{
		public:
			IndexBuffer(uint32_t* indices, size_t indicesSize);
			IndexBuffer() {}

			VkBuffer& getVkBuffer();

			size_t getIndicesSize();

			void destroy();
		private:

			size_t m_indicesSize = 0;

			static VkCommandPool m_commandPool;
			static int64_t m_bufferCount;

			Context& m_context = Context::get();
			VkBuffer m_buffer;
			VkDeviceMemory m_bufferMemory;
		};
	}
}