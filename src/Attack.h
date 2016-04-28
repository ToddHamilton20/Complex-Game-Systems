// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Attack derived class. Used for any temporary projectile or indicator.

#pragma once

#include "GameObject.h"

class GameObjects;
class Zombie;

class Attack : public GameObject
{
public:
	Attack(float a_timer) : alive(true), timer(a_timer), velocity(0, 0), owner(nullptr) {}
	virtual void Update(GameObjects* a_gameObjects, float a_deltaTime);

	float timer;
	bool alive;
	glm::vec2 velocity;
	Zombie* owner;
};