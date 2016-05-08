#include "Zombie.h"

#include "GLFW\glfw3.h"
#include <iostream>
#include "ZombieGenetics.h"
#include "GameObjects.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

void Zombie::Update(GameObjects* a_gameObjects, float a_deltaTime)
{
	if (alive && active)
	{
		glm::vec2 difference = position - a_gameObjects->player->position;
		glm::vec2 normalizeDifference = glm::normalize(difference);
		float distance = glm::length(difference);

		if (specialTraits & ZOMBIE_BIT_TRAIT_TELEPORT && teleportTimer.Ready() && distance > 400.0f && distance < 500.0f && !charging)
			Teleport(a_gameObjects, a_deltaTime);

		if (chargeDurationTimer.Ready() && charging)
			charging = false;

		if (specialTraits & ZOMBIE_BIT_TRAIT_CHARGE && chargeTimer.Ready() && chargeDurationTimer.Ready() && distance > 300.0f && distance < 500.0f)
			Charge(a_gameObjects, a_deltaTime);

		float oldSpeed = speed;

		if (charging)
			speed = 50.0f;

		glm::vec2 move = -normalizeDifference * glm::min((float)speed * a_deltaTime * 10.0f, distance - size.x / 2.0f - a_gameObjects->player->size.x / 2.0f);

		position += move;

		if (distance > a_gameObjects->player->size.x / 2.0f + size.x / 2.0f)
			rotation = glm::atan(move.x, -move.y) * 180 / glm::pi<float>();

		if (charging)
			speed = oldSpeed;

		if (distance <= a_gameObjects->player->size.x / 2.0f + size.x / 2.0f + 1.0f && hitTimer.Ready())
		{
			if (specialTraits & ZOMBIE_BIT_TRAIT_SHOCKWAVE)
				KnockBack(a_gameObjects, a_deltaTime);

			damageToPlayer = damageToPlayer + strength;
			a_gameObjects->player->DamagePlayer(ZOMBIE_DAMAGE, strength);
			hitTimer.Start(1.0f);
		}
		else if (distance < 600.0f && distance > 300.0f && hitTimer.Ready() && specialTraits & ZOMBIE_BIT_TRAIT_ACIDBREATH)
			SpawnProjectile(a_gameObjects, a_deltaTime);

		if (specialTraits & ZOMBIE_BIT_TRAIT_FIRETRAIL && glm::length(position - lastFire) >= 75.0f)
			SpawnFire(a_gameObjects, a_deltaTime);

		if (specialTraits & ZOMBIE_BIT_TRAIT_HEALING && health <= maxHealth / 2.0f && healingTimer.Ready())
			Heal(a_gameObjects, a_deltaTime);

		if (specialTraits & ZOMBIE_BIT_TRAIT_TRANSPARENT && transparentTimer.Ready() && transparentDurationTimer.Ready() && distance < 600 && distance > 400)
			GoInvisible(a_gameObjects, a_deltaTime);

		if (transparentDurationTimer.Ready())
			transparency = 1.0f;

		if (distance < 300.0f && !inRange)
		{
			rangeTimer.Start();
			inRange = true;
		}

		if (alive && health <= 0)
			alive = false;

		if (!alive && !spawnedSplitters && parent == nullptr && specialTraits & ZOMBIE_BIT_TRAIT_SPLITTING)
			SpawnSplitters(a_gameObjects, a_deltaTime);

		if (parent != nullptr)
		{
			parent->damageToPlayer = parent->damageToPlayer + damageToPlayer;
			damageToPlayer = 0.0f;
			parent->timeAlive = parent->timeAlive + timeAlive;
			timeAlive = 0.0f;
		}

		healthBar.UpdateHealth(health);

		hitTimer.Update(a_deltaTime);
		chargeTimer.Update(a_deltaTime);
		chargeDurationTimer.Update(a_deltaTime);
		teleportTimer.Update(a_deltaTime);
		healingTimer.Update(a_deltaTime);
		transparentTimer.Update(a_deltaTime);
		transparentDurationTimer.Update(a_deltaTime);
		rangeTimer.Update(a_deltaTime);
	}

	if (inRange && (!alive || !active))
	{
		rangeTimer.Stop();
		inRange = false;
		timeAlive = timeAlive + rangeTimer.GetTime();
	}
}

void Zombie::Draw(Camera* a_camera, int a_shader) const
{
	if (alive && active)
		sprite.DrawAt(a_camera, a_shader, position, size, rotation, transparency);
}

void Zombie::DrawHealth(Camera* a_camera, int a_shader) const
{
	if (alive && transparentDurationTimer.Ready() && active)
		healthBar.Draw(a_shader, GetScreenPosition(a_camera) - glm::vec2(0, size.y * 0.8f), glm::vec2(size.x, size.x / 5.0f));
}

void Zombie::Respawn()
{
	alive = true;
	health = maxHealth;
	damageToPlayer = 0;
	timeAlive = 0;
	spawnedSplitters = false;
	inRange = false;

	healthBar.Init(maxHealth, health);
}

void Zombie::SpawnSplitters(GameObjects* a_gameObjects, float a_deltaTime)
{
	int positionInt = rand() % 4;
	glm::vec2 newPosition;
	switch (positionInt)
	{
	case 0: newPosition = glm::vec2(-50, 0); break;
	case 1: newPosition = glm::vec2(-35, -35); break;
	case 2: newPosition = glm::vec2(0, -50); break;
	case 3: newPosition = glm::vec2(35, -35); break;
	}

	for (int j = -1; j < 2; j += 2)
	{
		Zombie* zombie = new Zombie();

		zombie->position = position + newPosition * j;
		zombie->parent = this;
		zombie->sprite = sprite;
		zombie->healthBar.sprite = healthBar.sprite;
		zombie->healthBar.backSprite = healthBar.backSprite;
		zombie->projectileSprite = projectileSprite;
		zombie->fireSprite = fireSprite;
		zombie->healSprite = healSprite;

		zombie->maxHealth = maxHealth / 2.0f;
		zombie->size = size / 2.0f;
		zombie->strength = strength / 2.0f;
		zombie->speed = speed / 2.0f;
		zombie->specialTraits = int(specialTraits);

		zombie->Respawn();
		zombie->active = true;
		a_gameObjects->zombies.push_back(zombie);
	}

	spawnedSplitters = true;
}

void Zombie::SpawnProjectile(GameObjects* a_gameObjects, float a_deltaTime)
{
	hitTimer.Start(1.5f);

	glm::vec2 velocity = glm::normalize(a_gameObjects->player->position - position) * 500.0f;
	Attack* projectile = new Attack(10.0f, velocity);
	projectile->position = position;
	projectile->size = glm::vec2(75, 75);
	projectile->sprite = projectileSprite;
	projectile->owner = this;

	a_gameObjects->projectiles.push_back(projectile);
}

void Zombie::SpawnFire(GameObjects* a_gameObjects, float a_deltaTime)
{
	lastFire = position;

	Attack* fire = new Attack(20.0f);
	fire->position = position;
	fire->size = glm::vec2(75, 75);
	fire->sprite = fireSprite;
	fire->owner = this;

	a_gameObjects->fires.push_back(fire);
}

void Zombie::KnockBack(GameObjects* a_gameObjects, float a_deltaTime)
{
	a_gameObjects->player->KnockBack(glm::normalize(a_gameObjects->player->position - position));
}

void Zombie::Heal(GameObjects* a_gameObjects, float a_deltaTime)
{
	health += maxHealth / 3.0f;
	healingTimer.Start(5.0f);

	Attack* heal = new Attack(2.0f, glm::vec2(0.0f, -15.0f));
	heal->position = position;
	heal->size = glm::vec2(75, 75);
	heal->sprite = healSprite;

	a_gameObjects->heals.push_back(heal);
}

void Zombie::Charge(GameObjects* a_gameObjects, float a_deltaTime)
{
	chargeTimer.Start(15.0f);
	chargeDurationTimer.Start(0.6f);
	charging = true;
}

void Zombie::GoInvisible(GameObjects* a_gameObjects, float a_deltaTime)
{
	transparency = 0.05f;
	transparentDurationTimer.Start(2.0f);
	transparentTimer.Start(15.0f);
}

void Zombie::Teleport(GameObjects* a_gameObjects, float a_deltaTime)
{
	float radius = glm::length(position - a_gameObjects->player->position);
	float degree = rand() % +91 + 45 + 180 + a_gameObjects->player->rotation;

	position = glm::vec2(
		a_gameObjects->player->position.x + radius * glm::cos(glm::radians(degree)),
		a_gameObjects->player->position.y + radius * glm::sin(glm::radians(degree)));

	teleportTimer.Start(15.0f);
}