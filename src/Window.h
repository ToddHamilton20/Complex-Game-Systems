// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Window singleton class

#pragma once

#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"

class Window
{
public:
	Window() {}
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static Window& GetInstance();

	bool InitialiseWindow(const char* a_title, int a_width, int a_height);
	void DestroyWindow();
	void SwapBuffers() const;
	int GetKey(const int a_key) const;
	int GetMouseButton(const int a_mouseButtom) const;
	void GetMousePos(float* a_xPos, float* a_yPos) const;
	bool ShouldClose() const;
	void GetSize(float* a_width, float* a_height) const;

private:
	GLFWwindow* window;
	float width, height;
};