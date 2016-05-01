#include "Window.h"

Window& Window::GetInstance()
{
	static Window instance;
	return instance;
}

bool Window::InitialiseWindow(const char* a_title, int a_width, int a_height)
{
	if (glfwInit() == GL_FALSE)
		return false;

	window = glfwCreateWindow(a_width, a_height, a_title, nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });

	glClearColor(0.25f, 0.25f, 0.25f, 1);

	glDepthFunc(GL_NEVER);
	glEnable(GL_CULL_FACE);

	width = a_width;
	height = a_height;

	return true;
}

void Window::DestroyWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(window);
}

int Window::GetKey(const int a_key) const
{
	return glfwGetKey(window, a_key);
}

int Window::GetMouseButton(const int a_mouseButton) const
{
	return glfwGetMouseButton(window, a_mouseButton);
}

bool Window::ShouldClose() const
{
	if (glfwWindowShouldClose(window))
		return true;

	return false;
}

void Window::GetMousePos(float* a_xPos, float* a_yPos) const
{
	double xPos = 0, yPos = 0;
	glfwGetCursorPos(window, &xPos, &yPos);

	*a_xPos = (float)xPos;
	*a_yPos = (float)yPos;
}

void Window::GetSize(float* a_width, float* a_height) const
{
	*a_width = width;
	*a_height = height;
}