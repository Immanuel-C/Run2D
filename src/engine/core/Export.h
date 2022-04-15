#pragma once

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <string>

#include "Window.h"
#include "VkContext.h"
#include "Timer.h"

namespace Run {
	typedef std::vector<const char*> CommandLineArgs;

	class Application {
	public:
		virtual void onStart(CommandLineArgs& args) {}
		virtual void onUpdate(double dt) {}
		virtual void onDestroy() {}
	};
}

Run::Application* getClientApplication();


void start(Run::Application* app, int argc, char** argv) {
	Run::CommandLineArgs args;

	for (uint32_t i = 0; i < argc; i++) 
		args.push_back(argv[i]);


	app->onStart(args);
}

double startTime = glfwGetTime(), endTime = 0.f;

void update(Run::Application* app) {
	startTime = glfwGetTime();
	app->onUpdate(startTime - endTime);
	endTime = startTime;
}


void destroy(Run::Application* app, Run::Vk::Context& context) {

	app->onDestroy();
	context.destroy();

	delete app;
}

int main(int argc, char** argv) {
	Run::Application* app = getClientApplication();

	Run::Vk::Context& context = Run::Vk::Context::get();

	Run::Window& window = Run::Window::get();

	start(app, argc, argv);

	while (!window.shouldClose())
		update(app);

	destroy(app, context);
}