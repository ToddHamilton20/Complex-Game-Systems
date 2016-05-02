#include "MenuApplication.h"

#include "Defines.h"
#include "Camera.h"
#include "Shader.h"
#include "gl_core_4_4.h"
#include "Window.h"
#include "Engine.h"
#include "EvolutionApplication.h"

bool MenuApplication::Startup()
{
	shader = Shader::CreateShader("Shader.vert", "Shader.frag");
	glUseProgram(shader);

	backgroundSprite.Load("Background.png");
	playButtonSprite.Load("PlayButton.png");
	exitButtonSprite.Load("ExitButton.png");

	playButton.Init(glm::vec2(SCREEN_X / 2.0f, SCREEN_Y / 2.0f - 100), glm::vec2(250, 100));
	exitButton.Init(glm::vec2(SCREEN_X / 2.0f, SCREEN_Y / 2.0f + 100), glm::vec2(250, 100));
	
	playButton.sprite = playButtonSprite;
	exitButton.sprite = exitButtonSprite;

	return true;
}

void MenuApplication::Shutdown()
{
	backgroundSprite.DestroySprite();
	playButtonSprite.DestroySprite();
	exitButtonSprite.DestroySprite();

	Sprite::DestroyMesh();
}

bool MenuApplication::Update(float deltaTime)
{
	if (Window::GetInstance().ShouldClose())
		engine->Quit();

	playButton.Update();
	exitButton.Update();

	if (playButton.GetState() == BUTTON_PRESSED)
	{
		BaseApplication* game = new EvolutionApplication(engine);
		engine->PushState(game);
	}
	else if (exitButton.GetState() == BUTTON_PRESSED)
		engine->PopState();

	return true;
}

void MenuApplication::Draw()
{
	backgroundSprite.DrawAtScreen(shader, glm::vec2(SCREEN_X / 2.0f, SCREEN_Y / 2.0f), glm::vec2(SCREEN_X, SCREEN_Y));
	playButton.Draw(shader);
	exitButton.Draw(shader);
}