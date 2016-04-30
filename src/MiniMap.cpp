#include "MiniMap.h"

#include "Camera.h"
#include "gl_core_4_4.h"
#include "Defines.h"

void MiniMap::Init(glm::vec2 a_dimensions, glm::vec2 a_size, glm::vec2 a_position)
{
	dimensions = a_dimensions;
	size = a_size;
	position = a_position;

	// Frame Buffer Setup
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, a_dimensions.x, a_dimensions.y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboTexture, 0);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	miniMapSprite.Init(true);

	clearSprite.Init("Darken.png");
}

void MiniMap::Draw(const GameObjects& a_gameObjects, Camera* a_camera, unsigned int a_worldShader, unsigned int a_miniMapShader)
{
	// Render world to minimap
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, dimensions.x, dimensions.y);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < a_gameObjects.arenaTiles.size(); i++)
	{
		a_gameObjects.arenaTiles[i]->Draw(a_camera, a_worldShader);
	}

	for (int i = 0; i < a_gameObjects.fires.size(); i++)
	{
		a_gameObjects.fires[i]->Draw(a_camera, a_worldShader);
	}

	for (int i = 0; i < a_gameObjects.zombies.size(); i++)
	{
		a_gameObjects.zombies[i]->Draw(a_camera, a_worldShader);
	}

	a_gameObjects.player->Draw(a_camera, a_worldShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_X, SCREEN_Y);

	// Render darken
	clearSprite.DrawAtScreen(a_miniMapShader, glm::vec2(SCREEN_X / 2.0f, SCREEN_Y / 2.0f), glm::vec2(SCREEN_X, SCREEN_Y));

	// Render minimap
	glUseProgram(a_miniMapShader);

	miniMapSprite.textureHandle = fboTexture;

	miniMapSprite.DrawAtScreen(a_miniMapShader, position, size);
}

void MiniMap::Destroy()
{
	clearSprite.Destroy();
	miniMapSprite.Destroy();
	glDeleteFramebuffers(1, &fbo);
}