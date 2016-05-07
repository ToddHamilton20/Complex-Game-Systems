// Author:					Todd Hamilton
// Last Change:				03/05/16
// Description of File:		Button class. 

#pragma once

#include "glm\glm.hpp"
#include "Sprite.h"

enum ButtonState
{
	BUTTON_IDLE,
	BUTTON_PRESSED,
	BUTTON_RELEASED,
	BUTTON_HOVERED
};

class Button
{
public:
	Button() {}
	void Init(glm::vec2 a_position, glm::vec2 a_size);
	void Update();
	void Draw(int a_shader);
	int GetState();

	Sprite sprite;

private:
	glm::vec2 position, size;
	int state;
};