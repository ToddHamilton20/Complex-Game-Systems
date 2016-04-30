// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Minimap class

#pragma once

#include "glm\glm.hpp"
#include "GameObjects.h"
#include "Sprite.h"

class Camera;

// MiniMap class. Init must be called before use, Destroy must be called after use. Will be rendered at position and size specified in Init.
class MiniMap
{
public:
	MiniMap() : dimensions(0, 0), position(0, 0), size(0, 0) {}

	// a_dimensions - Size of minimap internal texture
	// a_size - Size of rendered minimap in pixels
	// a_position - Position of rendered minimap in screen pixels.
	void Init(glm::vec2 a_dimensions, glm::vec2 a_size, glm::vec2 a_position);
	void Draw(const GameObjects& a_gameObjects, Camera* a_camera, unsigned int a_worldShader, unsigned int a_miniMapShader);
	void Destroy();

private:
	glm::vec2 dimensions, position, size;
	unsigned int fbo;
	unsigned int fboTexture;
	Sprite miniMapSprite, clearSprite;
};