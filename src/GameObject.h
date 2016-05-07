// Author:					Todd Hamilton
// Last Change:				08/05/16
// Description of File:		GameObject class and UIGameObject class

#pragma once

#include "glm/glm.hpp"
#include "Sprite.h"

class GameObjects;

// Pure virtual base GameObject class, all GameObjects should derive from this
class GameObject
{
public:
	GameObject() : position(0, 0), size(0, 0), rotation(0), transparency(1.0f) {}
	virtual ~GameObject() = 0;
	virtual void Update(GameObjects* a_gameObjects, float a_deltaTime) {}
	virtual void Draw(Camera* a_camera, int a_shader) const;
	virtual const glm::vec2 GetScreenPosition(Camera* a_camera) const;

	// Position in pixels - Game Space
	glm::vec2 position;
	// Size in pixels
	glm::vec2 size;
	// Rotation in degrees
	float rotation;
	Sprite sprite;
	// Normalised transparency
	float transparency;
};

// Pure virtual base GameObject class, all GameObjects should derive from this
class UIGameObject : public GameObject
{
	// Camera may be nullptr, is not used. Just overriding
	virtual void Draw(Camera* a_camera, int a_shader) const;
	// Camera should be nullptr, is not used. Just overriding
	const glm::vec2 GetScreenPosition(Camera* a_camera = nullptr) const;
};