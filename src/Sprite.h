// Author:					Todd Hamilton
// Last Change:				01/05/16
// Description of File:		Sprite class

#pragma once

#include <string>
#include "glm\glm.hpp"

class Camera;

// Sprite class, Init must be called before use, Destroy must be called after use. Copying class will create a shallow copy.
class Sprite
{
public:
	Sprite();

	void Load(bool a_yflip = false);
	void Load(const char* a_filePath, bool a_yflip = false);
	void Load(unsigned int a_texture, bool a_yflip = false);
	void DestroySprite();
	static void DestroyMesh();

	void DrawAt(Camera* a_camera, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;
	void DrawAtScreen(int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;
	
	unsigned int textureHandle;

private:
	bool flipped;

	static bool initialised;
	static unsigned int vbo, ibo, vao, vboFlipped, vaoFlipped;
};