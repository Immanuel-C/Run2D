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

			void destroy();
		private:
			Context& m_context = Context::get();
			VkBuffer m_buffer;
		};
	}
}
