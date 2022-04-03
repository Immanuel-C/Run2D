#pragma once

#include <GLFW/glfw3.h>

#include <vector>

namespace Run {
    namespace Vk {
        struct QueueFamilyIndices {
            int graphicsFamilyIdx = -1;
            int presentFamilyIdx = -1;
            int transferFamilyIdx = -1;

            bool isValid() {
                return graphicsFamilyIdx > -1 && presentFamilyIdx > -1 && transferFamilyIdx > -1;
            }
        };

        struct Queues {
            VkQueue graphics;
            VkQueue present;
            VkQueue transfer;
        };

        struct SwapChainDetails {
            VkSurfaceCapabilitiesKHR capabilities{};
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        struct Shader {
            VkShaderModule vertex;
            VkShaderModule fragment;
        };


        // Not in use 
        enum struct ShaderLanguage {
            GLSL,
            HLSL,

            MAX,
        };
    }
}