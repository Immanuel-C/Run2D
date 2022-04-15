#include "VkContext.h"
#include "VkGraphicsPipeline.h"
#include "VkRenderer.h"
#include "Window.h"
#include "VkVertexBuffer.h"
#include "Matrix.h"
#include "Export.h"
#include "Entity.h"
#include "Components.h"


class Game : public Run::Application {
public:
    Run::Window& window;

    Run::Vk::GraphicsPipeline graphicsPipeline;

    Run::Vk::Renderer renderer;

    std::array<Run::Vk::Vertex, 6> vertices{
        Run::Vk::Vertex{ {-0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f } }, // TL  
        Run::Vk::Vertex{ { 0.5f,  0.5f }, { 1.0f, 0.2f, 0.1f } }, // BR
        Run::Vk::Vertex{ {-0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f } }, // BL
        Run::Vk::Vertex{ { 0.5f, -0.5f }, { 1.0f, 0.4f, 0.0f } }, // TR  
    };

    std::array<uint32_t, 6> indices{
        0, 1, 2, // First  Triangle
        3, 1, 0  // Second Triangle
    };


    std::array<Run::Vk::IndexBuffer, 1> indexBuffers{
        Run::Vk::IndexBuffer{ indices.data(), indices.size() }
    };

    std::array<Run::Vk::VertexBuffer, 1> vertexBuffers{
        Run::Vk::VertexBuffer{ vertices.data(), vertices.size() }
    };

    Run::Entity e{};

    Game() : window { Run::Window::get() } {}

    void onStart(Run::CommandLineArgs& args) override {
        Run::Colour windowColour{ 1.0f, 0.5f, 0.25f, 1.0f };

        Run::Cursor cursor = Run::FileUtils::loadCursor("assets/textures/cursor.png");

        Run::Icon icon = Run::FileUtils::loadIcon("assets/textures/icon.png");

        window = Run::Window::get();

        graphicsPipeline = Run::Vk::GraphicsPipeline {
            "assets/shaders/bin/BasicShader.glsl.vert.spv",
            "assets/shaders/bin/BasicShader.glsl.frag.spv"
        };

        renderer = Run::Vk::Renderer{ graphicsPipeline };

        window.setSize(1280, 720)
            .setTitle("Run Game Engine")
            .setColour(windowColour)
            .setCursor(cursor, 0, cursor.height / 2)
            .setIcon(icon)
            .setClipboardString("Yo")
            .center();
    }

    void onUpdate(double dt) override {
        //I_LOG_INFO("Delta Time: %f", dt);

        renderer.draw(vertexBuffers.data(), vertexBuffers.size(), indexBuffers.data(), indexBuffers.size());

        if (window.isGamepadButtonPressed(Run::GamepadID::GP_1, Run::GamepadButtons::RightStick))
            window.setFullscreen(!window.isFullscreen());

        if (window.isKeyDown(Run::Keys::LeftControl, Run::Keys::V))
            I_LOG_INFO("Ctrl-V Pressed: %s", window.getClipboardString().c_str());

        if (window.pathsJustDropped()) 
            for (const char* path : window.getDroppedPaths()) 
                I_LOG_INFO("Dropped path | %s", path);

        window.getInputEvents();
    }

    void onDestroy() override {
        renderer.destroy();

        for (uint32_t i = 0; i < indexBuffers.size(); i++) {
            vertexBuffers[i].destroy();
            indexBuffers[i].destroy();
        }

        graphicsPipeline.destroy();
        window.destroy();
    }
};

Run::Application* getClientApplication() {
    return new Game;
}
