// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Player class

#pragma once

#include "GameObject.h"
#include "Timer.h"
#include "DamageTypes.h"
#include "HealthBar.h"

class GameObjects;
class Camera;

// Player class, Init must be called before use
class Player : public GameObject
{
public:
	Player() {}
	void Init(float a_speed, float a_strength, float a_health, float a_maxHealth);
	void Update(GameObjects* a_gameObjects, float a_deltaTime);
	void Draw(Camera* a_camera, int a_shader) const;
	void DrawHealth(Camera* a_camera, int a_shader) const;
	void DamagePlayer(DamageType a_damageType, float a_damage);
	void KnockBack(glm::vec2 a_direction);

	float speed, strength;
	bool alive;

	HealthBar healthBar;
	// Screen camera. Needed for determining direction to mouse from player when attacking.
	Camera* camera;
	Sprite swipeSprite;

private:
	float health, maxHealth;
	Cooldown attackTimer;
	Cooldown fireTimer;
	Cooldown knockbackTimer, knockBackDurationTimer;
	glm::vec2 knockBackDirection;
};