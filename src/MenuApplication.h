// Author:					Todd Hamilton
// Last Change:				03/05/16
// Description of File:		MenuApplication Class

#pragma once

#include "BaseApplication.h"
#include "Sprite.h"
#include "Button.h"

class Camera;

class MenuApplication : public BaseApplication
{
public:
	MenuApplication(Engine* a_engine) : BaseApplication(a_engine) {}

	bool Startup();
	void Shutdown();

	bool Update(float deltaTime);
	void Draw();

private:
	Sprite playButtonSprite, exitButtonSprite, backgroundSprite;
	Button playButton, exitButton;
	unsigned int shader;
};