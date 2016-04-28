#include "Sprite.h"

#include "gl_core_4_4.h"
#include "Camera.h"
#include <iostream>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Vertex
{
	glm::vec2 position;
	glm::vec2 texCoords;
};

void Sprite::Init(bool a_yflip)
{
	if (initialised)
	{
		std::cout << "ERROR: Sprite initialsied multiple times" << std::endl;
		return;
	}

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenVertexArrays(1, &vao);

	Vertex* vertices = new Vertex[4];
	vertices[0].position = { -0.5f, -0.5f };
	vertices[1].position = { -0.5f, 0.5f };
	vertices[2].position = { 0.5f, -0.5f };
	vertices[3].position = { 0.5f, 0.5f };

	if (a_yflip)
	{
		vertices[0].texCoords = { 0, 1 };
		vertices[1].texCoords = { 0, 0 };
		vertices[2].texCoords = { 1, 1 };
		vertices[3].texCoords = { 1, 0 };
	}
	else
	{
		vertices[0].texCoords = { 0, 0 };
		vertices[1].texCoords = { 0, 1 };
		vertices[2].texCoords = { 1, 0 };
		vertices[3].texCoords = { 1, 1 };
	}

	unsigned int* indices = new unsigned int[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)));

	glBindVertexArray(0);

	delete[] indices;
	delete[] vertices;

	initialised = true;
}

void Sprite::Init(const char* a_filePath, bool a_yflip)
{
	Init(a_yflip);

	int x = 0, y = 0, comp = 0;
	unsigned char* data = stbi_load(a_filePath, &x, &y, &comp, STBI_default);

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
}

void Sprite::Init(unsigned int a_texture, bool a_yflip)
{
	Init(a_yflip);

	textureHandle = a_texture;
}

void Sprite::Destroy()
{
	if (!initialised)
	{
		std::cout << "ERROR: Sprite not initialised" << std::endl;
		return;
	}

	glDeleteTextures(1, &textureHandle);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Sprite::DrawAt(Camera* a_camera, int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation, float a_alpha) const
{
	if (!initialised)
	{
		std::cout << "ERROR: Sprite not initialised" << std::endl;
		return;
	}

	glm::mat4 model = glm::translate(glm::vec3(a_position, 0)) * glm::rotate(a_rotation * glm::pi<float>() / 180, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(a_size, 1));
	glUniformMatrix4fv(glGetUniformLocation(a_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(a_shader, "projectionView"), 1, GL_FALSE, glm::value_ptr(a_camera->getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(glGetUniformLocation(a_shader, "diffuseTexture"), 0);

	glUniform1f(glGetUniformLocation(a_shader, "alpha"), a_alpha);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::DrawAtScreen(int a_shader, const glm::vec2& a_position, const glm::vec2& a_size, float a_rotation, float a_alpha) const
{
	Camera* camera = new Camera(1280, 720);
	camera->SetPos(glm::vec2(640, 360));

	if (!initialised)
	{
		std::cout << "ERROR: Sprite not initialised" << std::endl;
		return;
	}

	glm::mat4 model = glm::translate(glm::vec3(a_position, 0)) * glm::rotate(a_rotation * glm::pi<float>() / 180, glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(a_size, 1));
	glUniformMatrix4fv(glGetUniformLocation(a_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(a_shader, "projectionView"), 1, GL_FALSE, &camera->getProjectionView()[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(glGetUniformLocation(a_shader, "diffuseTexture"), 0);

	glUniform1f(glGetUniformLocation(a_shader, "alpha"), a_alpha);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	delete camera;
}