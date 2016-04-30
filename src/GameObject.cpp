#include "GameObject.h"

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "Camera.h"
#include "Window.h"

GameObject::~GameObject() {}

const glm::vec2 GameObject::GetScreenPosition(Camera* a_camera) const
{
	float width, height;
	Window::GetInstance().GetSize(&width, &height);
	glm::vec4 clipSpace = a_camera->GetProjectionView() * glm::vec4(position, 0, 1);
	glm::vec2 normalizedScreen = clipSpace.w != 0 ? glm::vec2(clipSpace.x, clipSpace.y) / clipSpace.w : glm::vec2(-1, -1);
	glm::vec2 windowSpace = ((normalizedScreen + glm::vec2(1, 1)) / 2.0f) * glm::vec2(width, height);

	windowSpace.y = height - windowSpace.y;

	return windowSpace;
}

void GameObject::Draw(Camera* a_camera, int a_shader) const
{
	Sprites& instance = Sprites::GetInstance();
	instance(sprite.c_str()).DrawAt(a_camera, a_shader, position, size, rotation, transparency);
}