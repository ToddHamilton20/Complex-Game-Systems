// Author:					Todd Hamilton
// Last Change:				01/05/16
// Description of File:		Camera class

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

// Orthographic camera class
class Camera
{
public:
	Camera(float a_width, float a_height);
	void SetPos(const glm::vec2& a_position);
	const glm::vec2 GetPos() const;

	const glm::mat4& GetTransform() const { return transform; }
	const glm::mat4& GetProjection() const { return projection; }
	const glm::mat4& GetView() const { return view; }
	const glm::mat4& GetProjectionView() const { return projectionView; }

private:
	glm::mat4 transform;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 projectionView;
};