// Author:					Todd Hamilton
// Last Change:				01/05/16
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
	Player() : alive(true), hurt(false) {}
	void Init(float a_speed, float a_strength, float a_health, float a_maxHealth);
	void Update(GameObjects* a_gameObjects, float a_deltaTime);
	void Draw(Camera* a_camera, int a_shader) const;
	void DrawHealth(Camera* a_camera, int a_shader) const;
	void DamagePlayer(DamageType a_damageType, float a_damage);
	void KnockBack(glm::vec2 a_direction);

	float speed, strength;
	bool alive;

	// True if the player got damaged and hit marker isn't spawned
	bool hurt;

	HealthBar healthBar;
	// Screen camera. Needed for determining direction to mouse from player when attacking.
	Camera* camera;
	Sprite swipeSprite, hitMarkerSprite;

private:
	float health, maxHealth;
	Cooldown attackTimer;
	Cooldown fireTimer;
	Cooldown knockbackTimer, knockBackDurationTimer;
	glm::vec2 knockBackDirection;
};