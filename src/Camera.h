#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Camera
{
public:
	Camera(float a_width, float a_height);
	void SetPos(const glm::vec2& a_position);
	const glm::vec2 GetPos() const;

	const glm::mat4&	getTransform()		const { return transform; }
	const glm::mat4&	getProjection()		const { return projection; }
	const glm::mat4&	getView()			const { return view; }
	const glm::mat4&	getProjectionView() const { return projectionView; }

private:
	glm::mat4 transform;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 projectionView;
};