#include "VkGraphicsPipeline.h"


namespace Run {
    namespace Vk {
        GraphicsPipeline::GraphicsPipeline(const std::string& vertPath, const std::string& fragPath) 
            : m_vertFileName { vertPath }, 
              m_fragFileName { fragPath }
        {

            createRenderPass();
            createShaders(vertPath, fragPath);
            createPipelineLayout();
            createPipeline();
        }

        VkRenderPass& GraphicsPipeline::getVkRenderPass() { return m_renderPass; }

        VkPipelineLayout& GraphicsPipeline::getVkPipelineLayout() { return m_pipelineLayout; }

        VkPipeline& GraphicsPipeline::getVkPipeline() { return m_pipeline; }

        std::string GraphicsPipeline::getVertexShaderFilePath() { return m_vertFileName; }

        std::string GraphicsPipeline::getFragmentShaderFilePath() { return m_fragFileName; }

        void GraphicsPipeline::destroy() {
            I_DEBUG_LOG_INFO("Destroying graphics pipeline... | RunEngine");

            I_DEBUG_LOG_INFO("Destroying shaders... | RunEngine");
            vkDestroyShaderModule(m_context.device, m_shader.vertex, nullptr);
            vkDestroyShaderModule(m_context.device, m_shader.fragment, nullptr);

            I_DEBUG_LOG_INFO("Destroying graphics pipeline layout... | RunEngine");    
            vkDestroyPipelineLayout(m_context.device, m_pipelineLayout, nullptr);
            I_DEBUG_LOG_INFO("Destroying render pass... | RunEngine");
            vkDestroyRenderPass(m_context.device, m_renderPass, nullptr);
            I_DEBUG_LOG_INFO("Destroying graphics pipeline... | RunEngine");    
            vkDestroyPipeline(m_context.device, m_pipeline, nullptr);

        }    


        void GraphicsPipeline::createRenderPass()
        {
            I_DEBUG_LOG_INFO("Creating render pass... | RunEngine");

            SwapChainDetails swapChainDetails = m_context.physicalDevice.getSwapChainDetails();

            VkFormat format = m_context.chooseBestSwapChainFormat(swapChainDetails.formats).format;

            VkAttachmentDescription colourAttachment{};
            colourAttachment.format = format;
            colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference colourAttachmentRef{};
            // index into array (e.g. we have two attachemnts and the colourAttachemnt is first in the
            // array than that will be the index of the colourAttachment).
            colourAttachmentRef.attachment = 0;
            colourAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpassDesc{};
            subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDesc.colorAttachmentCount = 1;
            subpassDesc.pColorAttachments = &colourAttachmentRef;

            VkSubpassDependency subpassDep{};
            subpassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpassDep.dstSubpass = 0;
            subpassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpassDep.srcAccessMask = 0;
            subpassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = 1;
            renderPassInfo.pAttachments = &colourAttachment;
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpassDesc;
            renderPassInfo.dependencyCount = 1;
            renderPassInfo.pDependencies = &subpassDep;

            VK_CHECK(vkCreateRenderPass(m_context.device, &renderPassInfo, nullptr, &m_renderPass));
        }



        void GraphicsPipeline::createShaders(const std::string& vertPath, const std::string& fragPath) {
            I_DEBUG_LOG_INFO("Creating a graphics pipeline with vertex shader: %s and fragment shader: %s | RunEngine", vertPath.c_str(), fragPath.c_str());
            
            std::vector<char> vertCode = FileUtils::read(vertPath, std::ios::binary);
            std::vector<char> fragCode = FileUtils::read(fragPath, std::ios::binary);

            VkShaderModuleCreateInfo vertexCreateInfo{}; 
            vertexCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            vertexCreateInfo.codeSize = vertCode.size();
            vertexCreateInfo.pCode = (uint32_t*)vertCode.data();


            VkShaderModuleCreateInfo fragmentCreateInfo{}; 
            fragmentCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            fragmentCreateInfo.codeSize = fragCode.size();
            fragmentCreateInfo.pCode = (uint32_t*)fragCode.data();

            VK_CHECK(vkCreateShaderModule(m_context.device, &fragmentCreateInfo, nullptr, &m_shader.fragment));
            VK_CHECK(vkCreateShaderModule(m_context.device, &vertexCreateInfo, nullptr, &m_shader.vertex));
        }

        void GraphicsPipeline::createPipelineLayout() {
            I_DEBUG_LOG_INFO("Creating graphics pipeline layout... | RunEngine");
            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

            VK_CHECK(vkCreatePipelineLayout(m_context.device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));
        }

        void GraphicsPipeline::createPipeline() {
            I_DEBUG_LOG_INFO("Creating graphics pipeline... | RunEngine");

            SwapChainDetails swapChainDetails = m_context.physicalDevice.getSwapChainDetails();

            VkExtent2D extent = m_context.chooseSwapChainExtent(swapChainDetails.capabilities);

            VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
            vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vertShaderStageInfo.module = m_shader.vertex;
            // Entry point
            vertShaderStageInfo.pName = "main";

            VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
            fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragShaderStageInfo.module = m_shader.fragment;
            fragShaderStageInfo.pName = "main";


            std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {
                vertShaderStageInfo,
                fragShaderStageInfo
            };

            auto bindingDesc = Vertex::getBindingDescription();
            auto attributeDesc = Vertex::getAttributeDescriptions();

            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
            vertexInputInfo.vertexAttributeDescriptionCount = attributeDesc.size();
            vertexInputInfo.pVertexAttributeDescriptions = attributeDesc.data();

            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
            inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
            

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = extent.width;
            viewport.height = extent.height;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = extent;
            
            VkPipelineViewportStateCreateInfo viewportInfo{};
            viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportInfo.viewportCount = 1;
            viewportInfo.pViewports = &viewport;
            viewportInfo.scissorCount = 1;
            viewportInfo.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
            rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizerInfo.depthClampEnable = VK_FALSE;
            rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
            rasterizerInfo.polygonMode = VK_POLYGON_MODE_LINE;
            rasterizerInfo.lineWidth = 1.0f;
            rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
            rasterizerInfo.depthBiasEnable = VK_FALSE;

            VkPipelineMultisampleStateCreateInfo multisamplingInfo{};
            multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisamplingInfo.sampleShadingEnable = VK_FALSE;
            multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_FALSE;
            colorBlendAttachment.blendEnable = VK_TRUE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

            VkPipelineColorBlendStateCreateInfo colorBlending{};
            colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlending.logicOpEnable = VK_FALSE;
            colorBlending.logicOp = VK_LOGIC_OP_COPY;
            colorBlending.attachmentCount = 1;
            colorBlending.pAttachments = &colorBlendAttachment;

            std::array<VkDynamicState, 2> dynamicStates = {
                    VK_DYNAMIC_STATE_VIEWPORT,
                    VK_DYNAMIC_STATE_SCISSOR
            };

            VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
            dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamicStateInfo.dynamicStateCount = dynamicStates.size();
            dynamicStateInfo.pDynamicStates = dynamicStates.data();


            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = shaderStages.size();
            pipelineInfo.pStages = shaderStages.data();
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
            pipelineInfo.pViewportState = &viewportInfo;
            pipelineInfo.pRasterizationState = &rasterizerInfo;
            pipelineInfo.pMultisampleState = &multisamplingInfo;
            pipelineInfo.pColorBlendState = &colorBlending;
            pipelineInfo.pDynamicState = &dynamicStateInfo;
            pipelineInfo.layout = m_pipelineLayout;
            pipelineInfo.renderPass = m_renderPass;
            pipelineInfo.subpass = 0;

            VK_CHECK(vkCreateGraphicsPipelines(m_context.device, nullptr, 1, &pipelineInfo, nullptr, &m_pipeline));
        }
    }
}