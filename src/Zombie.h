#pragma once

#include "GameObject.h"

#include "Timer.h"
#include "HealthBar.h"
#include <atomic>

class Player;
class GameObjects;

class Zombie : public GameObject
{
public:
	Zombie() : alive(false), active(false), damageToPlayer(0.0f), timeAlive(0.0f), inRange(false), spawnedSplitters(false), charging(false), parent(nullptr) {}

	void Update(GameObjects* a_gameObjects, float a_deltaTime);
	void Draw(Camera* a_camera, int a_shader) const;
	void DrawHealth(Camera* a_camera, int a_shader) const;

	glm::vec2 speedRange, strengthRange, maxHealthRange, scaleRange;
	std::atomic<float> speed, strength, maxHealth, scale;
	std::atomic<int> specialTraits;
	std::atomic<float> damageToPlayer, timeAlive;

	std::atomic<bool> alive;
	std::atomic<bool> active;
	float health;

	Zombie* parent;
	HealthBar healthBar;
	Sprite projectileSprite, fireSprite, healSprite;

	void Respawn();

private:
	glm::vec2 lastFire;
	Cooldown hitTimer;
	Cooldown chargeTimer, chargeDurationTimer;
	Cooldown teleportTimer;
	Cooldown healingTimer;
	Cooldown transparentTimer, transparentDurationTimer;
	Timer rangeTimer;
	bool spawnedSplitters, charging;
	bool inRange;

	void SpawnSplitters(GameObjects* a_gameObjects, float a_deltaTime);
	void SpawnProjectile(GameObjects* a_gameObjects, float a_deltaTime);
	void SpawnFire(GameObjects* a_gameObjects, float a_deltaTime);
	void KnockBack(GameObjects* a_gameObjects, float a_deltaTime);
	void Heal(GameObjects* a_gameObjects, float a_deltaTime);
	void Charge(GameObjects* a_gameObjects, float a_deltaTime);
	void GoInvisible(GameObjects* a_gameObjects, float a_deltaTime);
	void Teleport(GameObjects* a_gameObjects, float a_deltaTime);
};