// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Sprite class

#pragma once

#include <string>
#include "glm\glm.hpp"

class Camera;

// Sprite class, Init must be called before use, Destroy must be called after use. Copying class will create a shallow copy.
class Sprite
{
public:
	Sprite() : initialised(false) {}

	void Init(bool a_yflip = false);
	void Init(const char* a_filePath, bool a_yflip = false);
	void Init(unsigned int a_texture, bool a_yflip = false);
	void Destroy();

	void DrawAt(Camera* a_camera, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;
	void DrawAtScreen(int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;
	
	unsigned int textureHandle;

private:
	bool initialised;

	unsigned int vbo, ibo, vao;
};