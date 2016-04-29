// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Attack class

#pragma once

#include "GameObject.h"

class GameObjects;
class Zombie;

// Used for any gameobject with a constant velocity and despawn time.
class Attack : public GameObject
{
public:
	Attack(float a_timer, glm::vec2 a_velocity = glm::vec2(0, 0), bool a_fade = false) :
		alive(true), timer(a_timer), maxTimer(a_timer), velocity(a_velocity), fade(a_fade), owner(nullptr)
	{}

	virtual void Update(GameObjects* a_gameObjects, float a_deltaTime);

	bool alive;

	// Used to apply damage to owners' tally
	Zombie* owner;
	
private:
	float timer, maxTimer;
	glm::vec2 velocity;
	bool fade;
};