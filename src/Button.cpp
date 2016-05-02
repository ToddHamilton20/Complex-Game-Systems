#include "Button.h"

#include "Window.h"

void Button::Init(glm::vec2 a_position, glm::vec2 a_size)
{
	position = a_position;
	size = a_size;
}

void Button::Update()
{
	glm::vec2 mousePos;
	Window::GetInstance().GetMousePos(&mousePos.x, &mousePos.y);

	bool hovered = false;

	if (mousePos.x >= position.x - size.x / 2.0f &&
		mousePos.x <= position.x + size.x / 2.0f &&
		mousePos.y >= position.y - size.y / 2.0f &&
		mousePos.y <= position.y + size.y / 2.0f)
	{
		hovered = true;
	}

	if (Window::GetInstance().GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) && hovered)
		state = BUTTON_PRESSED;
	else if (state == BUTTON_PRESSED)
		state = BUTTON_RELEASED;
	else if (hovered)
		state = BUTTON_HOVERED;
	else
		state = BUTTON_IDLE;
}

void Button::Draw(int a_shader)
{
	sprite.DrawAtScreen(a_shader, position, size);
}

int Button::GetState()
{
	return state;
}