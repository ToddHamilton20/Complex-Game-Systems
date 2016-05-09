// Author:					Todd Hamilton
// Last Change:				08/05/16
// Description of File:		Zombie class

#pragma once

#include "GameObject.h"

#include "Timer.h"
#include "HealthBar.h"
#include <atomic>
#include <vector>

class Player;
class GameObjects;

struct ParentTraits
{
	ParentTraits(float a_fitness, float a_speed, float a_strength, float a_maxHealth, float a_scale, int a_specialTraits) : 
	fitness(a_fitness), speed(a_speed), strength(a_strength), maxHealth(a_maxHealth), scale(a_scale), specialTraits(a_specialTraits) {}

	float fitness;
	float speed, strength, maxHealth, scale;
	int specialTraits;
};

// Zombie class, Atomics are used as the genetic algorithm thread will refernece this class.
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

	// Vector to store parent traits, used for GUI only
	std::vector<ParentTraits> parentTraits;

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