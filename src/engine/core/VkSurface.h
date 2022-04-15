#pragma once

#include <GLFW/glfw3.h>
#include <ILog.h>
#include <vector>
#include <array>

#include "debug.h"
#include "VkInstance.h"
#include "VkStructs.h"
#include "DynamicExport.h"


namespace Run {
    namespace Vk {
        class RUN_API Surface {
        public:
            Surface(Instance& instance, GLFWwindow*& window);
            Surface() {}

            void destroy();

            operator VkSurfaceKHR&() { return m_surface; }
        private:
            VkSurfaceKHR m_surface{};
            Instance* m_instance{};
        };
    }
}