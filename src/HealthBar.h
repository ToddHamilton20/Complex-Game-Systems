// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		HealthBar class

#pragma once

#include "GameObject.h"

class HealthBar : public GameObject
{
public:
	void Init(float a_maxHealth, float a_health = -1.0f);
	virtual void UpdateHealth(float a_health);
	virtual void Draw(int a_shader, glm::vec2 a_position, glm::vec2 a_size) const;
	
	Sprite backSprite;

private:
	float health, maxHealth;
};