#include "VkContext.h"
#include "VkGraphicsPipeline.h"
#include "VkRenderer.h"
#include "Window.h"
#include "VkVertexBuffer.h"


int main(int argc, char** argv) {
    Run::Window& window = Run::Window::get();

    Run::Colour windowColour{ 1.0f, 0.5f, 0.25f, 1.0f };
    Run::Cursor cursor = Run::FileUtils::loadCursor("assets/textures/cursor.png");

    window.setSize(800, 600)
        .setTitle("Run Game Engine")
        .setColour(windowColour)
        .setCursor(cursor, 0, cursor.height / 2)
        .center();

    cursor.destroy();


    Run::Vk::Context& context = Run::Vk::Context::get();
    context.init(window);


    std::array<Run::Vk::Vertex, 6> vertices {
        Run::Vk::Vertex{ {-0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f } }, // TL  
        Run::Vk::Vertex{ { 0.5f,  0.5f }, { 1.0f, 0.2f, 0.1f } }, // BR
        Run::Vk::Vertex{ {-0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f } }, // BL
        Run::Vk::Vertex{ { 0.5f, -0.5f }, { 1.0f, 0.4f, 0.0f } }, // TR  
    };

    std::array<uint32_t, 6> indices{
        0, 1, 2, // First  Triangle
        3, 1, 0  // Second Triangle
    };

    std::array<Run::Vk::IndexBuffer, 1> indexBuffers {
        Run::Vk::IndexBuffer { indices.data(), indices.size() }
    };

    std::array<Run::Vk::VertexBuffer, 1> vertexBuffers {
        Run::Vk::VertexBuffer{ vertices.data(), vertices.size() }
    };


    Run::Vk::GraphicsPipeline graphicsPipeline {
        "assets/shaders/bin/BasicShader.glsl.vert.spv",
        "assets/shaders/bin/BasicShader.glsl.frag.spv"
    };

    Run::Vk::Renderer renderer { graphicsPipeline };


    while(!window.shouldClose()) {
        renderer.draw(vertexBuffers.data(), vertexBuffers.size(), indexBuffers.data(), indexBuffers.size());

        if (window.isKeyDown(GLFW_KEY_F)) {
            window.setFullscreen(!window.isFullscreen());
        }

        window.getInputEvents();
    }

    renderer.destroy();
    
    for (uint32_t i = 0; i < indexBuffers.size(); i++) {
        vertexBuffers[i].destroy();
        indexBuffers[i].destroy();
    }

    graphicsPipeline.destroy();
    context.destroy();
    window.destroy();
}