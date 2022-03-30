#include "VkContext.h"
#include "VkGraphicsPipeline.h"
#include "VkRenderer.h"
#include "Window.h"


int main(int argc, char** argv) {
    Run::Window& window = Run::Window::get();
    window.setSize(1280, 720);
    window.setTitle("Run Game Engine");
    window.setColour({1.0f, 0.5f, 0.25f, 1.0f});

    Run::Vk::Context& context = Run::Vk::Context::get();
    context.init(window);
    
    Run::Vk::GraphicsPipeline graphicsPipeline = {
        "assets/shaders/bin/BasicShader.glsl.vert.spv", 
        "assets/shaders/bin/BasicShader.glsl.frag.spv",
    };

    Run::Vk::Renderer renderer{ graphicsPipeline };

    while(!window.shouldClose()) {

        renderer.draw();

        window.getInputEvents();
    }

    renderer.destroy();
    graphicsPipeline.destroy();
    context.destroy();
    window.destroy();
}