#pragma once

#include "glm/glm.hpp"
#include "Sprite.h"

class GameObjects;

class GameObject
{
public:
	GameObject() : position(0, 0), size(0, 0), rotation(0), transparency(1.0f) {}
	virtual ~GameObject() = 0;
	virtual void Update(GameObjects* a_gameObjects, float a_deltaTime) {}
	virtual void Draw(Camera* a_camera, int a_shader) const;
	const glm::vec2 GetScreenPosition(Camera* a_camera) const;

	glm::vec2 position;
	glm::vec2 size;
	float rotation;
	Sprite sprite;
	float transparency;
};