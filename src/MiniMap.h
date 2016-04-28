// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Minimap class

#pragma once

#include "glm\glm.hpp"
#include "GameObjects.h"
#include "Sprite.h"

class Camera;

class MiniMap
{
public:
	MiniMap() : dimensions(0, 0), position(0, 0), size(0, 0) {}

	void Init(glm::vec2 a_dimensions, glm::vec2 a_size, glm::vec2 a_position);
	void Draw(const GameObjects& a_gameObjects, Camera* a_camera, unsigned int a_worldShader, unsigned int a_miniMapShader);

private:
	glm::vec2 dimensions, position, size;
	unsigned int fbo;
	unsigned int fboTexture;
	Sprite miniMapSprite, clearSprite;
};