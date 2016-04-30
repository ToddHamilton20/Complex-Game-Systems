// Author:					Todd Hamilton
// Last Change:				01/05/16
// Description of Change:	Commented
// Description of File:		Sprites class

#pragma once

#include "glm\glm.hpp"

#include <vector>

class Camera;

// Sprite class
struct Sprite
{
	Sprite() {}
	Sprite(const char* a_name, int a_id, int a_textureHandle, bool a_flipped) :
		name(a_name), id(a_id), textureHandle(a_textureHandle), flipped(a_flipped) {}
	Sprite(const char* a_name);
	Sprite(const char* a_id);

	void DrawAt(Camera* a_camera, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;
	void DrawAtScreen(int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;

	std::string name;
	unsigned int id;
	unsigned int textureHandle;
	bool flipped;
};

// Sprites singleton manager class.
class Sprites
{
public:
	Sprites();
	Sprites(const Sprites&) = delete;
	~Sprites();
	Sprites& operator=(const Sprites&) = delete;
	static Sprites& GetInstance();

	int InitSprite(const char* a_name, const char* a_filePath, bool a_yflip = false);
	int InitSprite(const char* a_name, unsigned int a_texture, bool a_yflip = false);

	void DestroySprite(int a_id);
	void DestroySprite(const char* a_name);
	void DestroySprites();

	void DrawAt(const Sprite& a_sprite, Camera* a_camera, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;
	void DrawAtScreen(const Sprite& a_sprite, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation = 0.0f, float a_alpha = 1.0f) const;

private:
	unsigned int vbo, vboFlipped, ibo, vao, vaoFlipped;

	std::vector<Sprite> sprites;
	int lastID;
};