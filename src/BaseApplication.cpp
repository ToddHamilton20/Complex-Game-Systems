#include "gl_core_4_4.h"
#include "BaseApplication.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Window.h"

void BaseApplication::run() {

	double prevTime = glfwGetTime();
	double currTime = 0;

	while (currTime = glfwGetTime(),
		update((float)(currTime - prevTime))) {

		glfwPollEvents();
		draw();

		Window::GetInstance().SwapBuffers();

		prevTime = currTime;
	}
}