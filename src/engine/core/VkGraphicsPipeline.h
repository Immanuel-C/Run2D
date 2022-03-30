#pragma once

#include <ILog.h>
#include <string>
#include <GLFW/glfw3.h>

#include "VkStructs.h"
#include "FileUtils.h"
#include "debug.h"
#include "VkContext.h"
#include "VkStructs.h"

namespace Run {
    namespace Vk {
        class GraphicsPipeline {
        public:
            GraphicsPipeline(const std::string& vertPath, const std::string& fragPath);

            VkRenderPass& getVkRenderPass();
            VkPipelineLayout& getVkPipelineLayout();
            VkPipeline& getVkPipeline();

            void destroy();
        private:
            void createRenderPass();
            void createShaders(const std::string& vertPath, const std::string& fragPath);
            void createPipeline();
            void createPipelineLayout();

            Shader m_shader{};
            VkPipelineLayout m_pipelineLayout;
            VkRenderPass m_renderPass;
            VkPipeline m_pipeline;
            Context& m_context = Context::get();
        };
    }
}