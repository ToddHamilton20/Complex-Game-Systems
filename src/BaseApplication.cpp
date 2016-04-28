#include "gl_core_4_4.h"
#include "BaseApplication.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Window.h"

void BaseApplication::Run() {

	double prevTime = glfwGetTime();
	double currTime = 0;

	while (currTime = glfwGetTime(),
		Update((float)(currTime - prevTime))) {

		glfwPollEvents();
		Draw();

		Window::GetInstance().SwapBuffers();

		prevTime = currTime;
	}
}