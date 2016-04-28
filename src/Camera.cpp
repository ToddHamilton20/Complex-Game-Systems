#include "Camera.h"

Camera::Camera(float a_width, float a_height)
{
	transform = glm::translate(glm::vec3(0, 0, -10));
	projection = glm::ortho<float>(-a_width / 2, a_width / 2, a_height / 2, -a_height / 2, -100, 100);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projectionView = projection * view;
}

void Camera::SetPos(const glm::vec2& a_position)
{
	transform = glm::translate(glm::vec3(a_position.x, a_position.y, -10));
	view = glm::lookAt(glm::vec3(a_position.x, a_position.y, 0.0f), glm::vec3(a_position.x, a_position.y, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projectionView = projection * view;
}

const glm::vec2 Camera::GetPos() const
{
	return glm::vec2(transform[3]);
}