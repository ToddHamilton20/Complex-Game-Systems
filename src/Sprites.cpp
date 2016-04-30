#include "Sprites.h"

#include "gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>

struct Vertex
{
	glm::vec2 position;
	glm::vec2 texCoords;
};

void Sprite::DrawAt(Camera* a_camera, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation, float a_alpha) const
{
	Sprites::GetInstance().DrawAt(*this, a_camera, a_shader, a_position, a_size, a_rotation, a_alpha);
}

void Sprite::DrawAtScreen(int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation, float a_alpha) const
{
	Sprites::GetInstance().DrawAtScreen(*this, a_shader, a_position, a_size, a_rotation, a_alpha);
}

Sprites::Sprites()
{
	lastID = -1;

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vboFlipped);
	glGenBuffers(1, &ibo);
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vaoFlipped);

	Vertex* vertices = new Vertex[4];
	vertices[0].position = { -0.5f, -0.5f };
	vertices[1].position = { -0.5f, 0.5f };
	vertices[2].position = { 0.5f, -0.5f };
	vertices[3].position = { 0.5f, 0.5f };

	Vertex* verticesFlipped = new Vertex[4];
	vertices[0].position = { -0.5f, -0.5f };
	vertices[1].position = { -0.5f, 0.5f };
	vertices[2].position = { 0.5f, -0.5f };
	vertices[3].position = { 0.5f, 0.5f };

	verticesFlipped[0].texCoords = { 0, 1 };
	verticesFlipped[1].texCoords = { 0, 0 };
	verticesFlipped[2].texCoords = { 1, 1 };
	verticesFlipped[3].texCoords = { 1, 0 };

	vertices[0].texCoords = { 0, 0 };
	vertices[1].texCoords = { 0, 1 };
	vertices[2].texCoords = { 1, 0 };
	vertices[3].texCoords = { 1, 1 };

	unsigned int* indices = new unsigned int[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	// Mesh
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)));

	// Flipped Mesh
	glBindVertexArray(vaoFlipped);

	glBindBuffer(GL_ARRAY_BUFFER, vboFlipped);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)));

	glBindVertexArray(0);

	delete[] indices;
	delete[] vertices;
	delete[] verticesFlipped;
}

Sprites::~Sprites()
{
	DestroySprites();

	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vaoFlipped);
	glDeleteBuffers(1, &vboFlipped);
	glDeleteBuffers(1, &ibo);
}

Sprites& Sprites::GetInstance()
{
	static Sprites instance;
	return instance;
}

int Sprites::InitSprite(const char* a_name, const char* a_filePath, bool a_yflip)
{
	int x = 0, y = 0, comp = 0;
	unsigned char* data = stbi_load(a_filePath, &x, &y, &comp, STBI_default);

	unsigned int textureHandle;
	if (data != nullptr)
	{
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		switch (comp)
		{
		case STBI_grey:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y,
				0, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
		case STBI_grey_alpha:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, x, y,
				0, GL_RG, GL_UNSIGNED_BYTE, data);
			break;
		case STBI_rgb:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y,
				0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case STBI_rgb_alpha:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:	break;
		};
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
		printf("Cannot load image");

	sprites.push_back(Sprite(a_name, ++lastID, textureHandle, a_yflip));

	return lastID;
}

int Sprites::InitSprite(const char* a_name, unsigned int a_texture, bool a_yflip)
{
	sprites.push_back(Sprite(a_name, ++lastID, a_texture, a_yflip));

	return lastID;
}

void Sprites::DestroySprite(int a_id)
{
	for (int i = sprites.size() - 1; i >= 0; i--)
	{
		if (sprites[i].id == a_id)
		{
			glDeleteTextures(1, &sprites[i].textureHandle);
			sprites.erase(std::remove(sprites.begin(), sprites.end(), sprites[i]), sprites.end());
		}
	}
}

void Sprites::DestroySprite(const char* a_name)
{
	for (int i = sprites.size() - 1; i >= 0; i--)
	{
		if (sprites[i].name == a_name)
		{
			glDeleteTextures(1, &sprites[i].textureHandle);
			sprites.erase(std::remove(sprites.begin(), sprites.end(), sprites[i]), sprites.end());
		}
	}
}

void Sprites::DestroySprites()
{
	for (int i = sprites.size() - 1; i >= 0; i--)
	{
		glDeleteTextures(1, &sprites[i].textureHandle);
		sprites.pop_back();
	}
}

void Sprites::DrawAt(const Sprite& a_sprite, Camera* a_camera, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation, float a_alpha) const
{
	glm::mat4 model = glm::translate(glm::vec3(a_position, 0)) * glm::rotate(a_rotation * glm::pi<float>() / 180, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(a_size, 1));
	glUniformMatrix4fv(glGetUniformLocation(a_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(a_shader, "projectionView"), 1, GL_FALSE, glm::value_ptr(a_camera->GetProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_sprite.textureHandle);
	glUniform1i(glGetUniformLocation(a_shader, "diffuseTexture"), 0);

	glUniform1f(glGetUniformLocation(a_shader, "alpha"), a_alpha);

	if (a_sprite.flipped)
		glBindVertexArray(vaoFlipped);
	else
		glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprites::DrawAtScreen(const Sprite& a_sprite, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation, float a_alpha) const
{
	Camera* camera = new Camera(1280, 720);
	camera->SetPos(glm::vec2(640, 360));

	glm::mat4 model = glm::translate(glm::vec3(a_position, 0)) * glm::rotate(a_rotation * glm::pi<float>() / 180, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(a_size, 1));
	glUniformMatrix4fv(glGetUniformLocation(a_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(a_shader, "projectionView"), 1, GL_FALSE, &camera->GetProjectionView()[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_sprite.textureHandle);
	glUniform1i(glGetUniformLocation(a_shader, "diffuseTexture"), 0);

	glUniform1f(glGetUniformLocation(a_shader, "alpha"), a_alpha);

	if (a_sprite.flipped)
		glBindVertexArray(vaoFlipped);
	else
		glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	delete camera;
}

Sprite& Sprites::operator()(const char* a_name)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		if (sprites[i].name == a_name)
			return sprites[i];
	}

	return sprites[0];
}

Sprite& Sprites::operator()(int a_id)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		if (sprites[i].id == a_id)
			return sprites[i];
	}

	return sprites[0];
}