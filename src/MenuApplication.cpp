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
	shader = Shader::CreateShader("../Resources/Shaders/Shader.vert", "../Resources/Shaders/Shader.frag");
	glUseProgram(shader);

	backgroundSprite.Load	("../Resources/Textures/Background.png");
	playButtonSprite.Load	("../Resources/Textures/PlayButton.png");
	exitButtonSprite.Load	("../Resources/Textures/ExitButton.png");

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