#include "VkContext.h"
#include "VkGraphicsPipeline.h"
#include "VkRenderer.h"
#include "Window.h"


int main(int argc, char** argv) {
    Run::Window& window = Run::Window::get();

    Run::Colour windowColour{ 1.0f, 0.5f, 0.25f, 1.0f };
    Run::Cursor cursor = Run::FileUtils::loadCursor("assets/textures/cursor.png");

    window.setSize(1280, 720)
        .setTitle("Run Game Engine")
        .setColour(windowColour)
        .setCursor(cursor, 0, cursor.height / 2);

    cursor.destroy();

    for (uint32_t i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == NULL) {
            window.setFullscreen(true);
        }
    }

    Run::Vk::Context& context = Run::Vk::Context::get();
    context.init(window);

    
    Run::Vk::GraphicsPipeline graphicsPipeline = {
        "assets/shaders/bin/BasicShader.glsl.vert.spv",
        "assets/shaders/bin/BasicShader.glsl.frag.spv"
    };

    Run::Vk::Renderer renderer{ graphicsPipeline };

    bool isFullscreen = false;

    while(!window.shouldClose()) {
        renderer.draw();

        if (window.isKeyDown(GLFW_KEY_F)) {
            isFullscreen = !isFullscreen;
            window.setFullscreen(isFullscreen);
        }


        window.getInputEvents();
    }

    renderer.destroy();
    graphicsPipeline.destroy();
    context.destroy();
    window.destroy();
}