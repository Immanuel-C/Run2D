#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>
#include <vector>
#include <array>
#include <set>

#include "debug.h"
#include "VkStructs.h"
#include "VkInstance.h"
#include "VkSurface.h"
#include "VkPhysicalDevice.h"
#include "DynamicExport.h"

namespace Run {
    namespace Vk {
        class RUN_API Device {
        public:
            Device(PhysicalDevice physicalDevice);
            Device() {}

            VkDevice& getVkDevice();
            Queues& getQueues();

            void destroy();

            operator VkDevice&() { return m_device; }
        private:
            VkDevice m_device{};
            Queues m_queues{};
        }; 
    }
}