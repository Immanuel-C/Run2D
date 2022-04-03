#pragma once

#include <GLFW/glfw3.h>
#include "VkContext.h"
#include "Timer.h"

namespace Run {
	namespace Vk {
		// Have to wrap in namespace because of linker errors
		namespace {
			uint32_t findMemoryType(Context& context, uint32_t typeFilter, VkMemoryPropertyFlags properties)
			{
				// Get the memory properties (which contains the memory types)
				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(context.physicalDevice, &memProperties);

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

			void createBuffer(Context& context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
			{
				std::set<uint32_t> queueFamilies = {
					(uint32_t)context.queueIndices.graphicsFamilyIdx,
					(uint32_t)context.queueIndices.transferFamilyIdx
				};

				VkBufferCreateInfo bufferInfo{};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				// Size in bytes
				bufferInfo.size = size;
				bufferInfo.usage = usage;
				if (queueFamilies.size() > 1) {
					std::vector<uint32_t> vQueueFamilies;
					vQueueFamilies.resize(queueFamilies.size());

					for (const uint32_t& queueFamily : queueFamilies)
						vQueueFamilies.push_back(queueFamily);

					bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
					bufferInfo.queueFamilyIndexCount = vQueueFamilies.size();
					bufferInfo.pQueueFamilyIndices = vQueueFamilies.data();
				}
				else {
					bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				}

				VK_CHECK(vkCreateBuffer(context.device, &bufferInfo, nullptr, &buffer));

				VkMemoryRequirements memRequirements{};
				vkGetBufferMemoryRequirements(context.device, buffer, &memRequirements);


				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(context, memRequirements.memoryTypeBits, properties);

				VK_CHECK(vkAllocateMemory(context.device, &allocInfo, nullptr, &bufferMemory));

				// Associate this memory with the buffer
				vkBindBufferMemory(context.device, buffer, bufferMemory, 0);

			}

			void bufcpy(Context& context, VkCommandPool& commandPool, VkBuffer& dstBuffer, VkBuffer srcBuffer, VkDeviceSize size)
			{
				VkCommandBufferAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = commandPool;
				allocInfo.commandBufferCount = 1;

				VkCommandBuffer transferCommandBuffer;
				VK_CHECK(vkAllocateCommandBuffers(context.device, &allocInfo, &transferCommandBuffer));

				VkCommandBufferBeginInfo beginInfo{};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

				VK_CHECK(vkBeginCommandBuffer(transferCommandBuffer, &beginInfo));

				VkBufferCopy copyRegion{};
				copyRegion.size = size;

				vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

				VK_CHECK(vkEndCommandBuffer(transferCommandBuffer));

				VkSubmitInfo submitInfo{};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &transferCommandBuffer;

				VK_CHECK(vkQueueSubmit(context.device.getQueues().transfer, 1, &submitInfo, nullptr));
				VK_CHECK(vkQueueWaitIdle(context.device.getQueues().transfer));

				vkFreeCommandBuffers(context.device, commandPool, 1, &transferCommandBuffer);
			}
		}
	}
}