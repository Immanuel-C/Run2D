#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>
#include <vector>
#include <array>
#include "debug.h"

namespace Run {
    namespace Vk {
        class Instance {
        public:
            Instance(uint32_t vkVersion);
            Instance() {}
            
            void destroy();

            VkInstance& getVkInstance();

            operator VkInstance&() { return m_instance; }
            operator VkInstance*() { return &m_instance; }
        private:
            bool checkInstanceLayerSupport(const char** layers, size_t layerSize);

            VkInstance m_instance{};
        };
    }
}