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
	Attack(float a_timer) : alive(true), timer(a_timer), velocity(0, 0), owner(nullptr) {}
	virtual void Update(GameObjects* a_gameObjects, float a_deltaTime);

	float timer;
	bool alive;
	glm::vec2 velocity;

	// Used to apply damage to owners' tally
	Zombie* owner;
};