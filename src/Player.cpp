#include "Player.h"

#include "Window.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <iostream>
#include "GameObjects.h"
#include "Defines.h"

void Player::Init(float a_speed, float a_strength, float a_health, float a_maxHealth)
{
	speed = a_speed;
	strength = a_strength;
	health = a_health;
	maxHealth = a_maxHealth;

	healthBar.Init(health, maxHealth);
}

void Player::Update(GameObjects* a_gameObjects, float a_deltaTime)
{
	if (alive)
	{
		glm::vec2 move(0, 0);

		if (knockBackDurationTimer.Ready() && knockbackTimer.Ready())
		{
			if (Window::GetInstance().GetKey(GLFW_KEY_UP) || Window::GetInstance().GetKey(GLFW_KEY_W))
				move += glm::vec2(0, -1);
			if (Window::GetInstance().GetKey(GLFW_KEY_DOWN) || Window::GetInstance().GetKey(GLFW_KEY_S))
				move += glm::vec2(0, 1);
			if (Window::GetInstance().GetKey(GLFW_KEY_LEFT) || Window::GetInstance().GetKey(GLFW_KEY_A))
				move += glm::vec2(-1, 0);
			if (Window::GetInstance().GetKey(GLFW_KEY_RIGHT) || Window::GetInstance().GetKey(GLFW_KEY_D))
				move += glm::vec2(1, 0);
		}
		else if (!knockBackDurationTimer.Ready())
			move = knockBackDirection * 4.0f;

		if (move != glm::vec2(0, 0))
		{
			if (knockBackDurationTimer.Ready())
				rotation = glm::atan(move.x, -move.y) * 180 / glm::pi<float>();
			position += glm::normalize(move) * speed * a_deltaTime;
		}

		if (position.x < -ARENA_SIZE / 2.0f - TILE_SIZE + size.x / 2.0f)
			position.x = -ARENA_SIZE / 2.0f - TILE_SIZE + size.x / 2.0f;
		if (position.y < -ARENA_SIZE / 2.0f - TILE_SIZE + size.x / 2.0f)
			position.y = -ARENA_SIZE / 2.0f - TILE_SIZE + size.x / 2.0f;

		position = glm::clamp(position,
			glm::vec2(-ARENA_SIZE / 2.0f - TILE_SIZE + size.x / 2.0f, -ARENA_SIZE / 2.0f - TILE_SIZE + size.y / 2.0f),
			glm::vec2(ARENA_SIZE / 2.0f + TILE_SIZE - size.x / 2.0f, ARENA_SIZE / 2.0f + TILE_SIZE - size.y / 2.0f));

		if ((Window::GetInstance().GetKey(GLFW_KEY_SPACE) || Window::GetInstance().GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) && attackTimer.Ready())
		{
			attackTimer.Start(0.5f);

			glm::vec2 mousePos;
			Window::GetInstance().GetMousePos(&mousePos.x, &mousePos.y);
			glm::vec2 direction = glm::normalize(mousePos - GetScreenPosition(camera));

			Attack* attack = new Attack("SwipeSprite", 0.1f);
			attack->position = position + direction * (size / 2.0f + 75.0f);
			attack->size = glm::vec2(150.0f, 150.0f);
			attack->rotation = glm::atan(direction.x, -direction.y) * 180 / glm::pi<float>();

			for (int i = 0; i < a_gameObjects->zombies.size(); i++)
			{
				if (!a_gameObjects->zombies[i]->active || !a_gameObjects->zombies[i]->alive)
					continue;

				if (glm::length(a_gameObjects->zombies[i]->position - attack->position) <= attack->size.x / 2.0f + a_gameObjects->zombies[i]->size.x / 2.0f)
					a_gameObjects->zombies[i]->health -= a_gameObjects->player->strength;
			}

			a_gameObjects->attacks.push_back(attack);
		}

		if (hurt)
		{
			Attack* attack = new Attack("HitMarker", 0.5f, glm::vec2(0, 0), true);
			attack->position = glm::vec2(SCREEN_X / 2.0f, SCREEN_Y / 2.0f);
			attack->size = glm::vec2(SCREEN_X, SCREEN_Y);
			a_gameObjects->hitMarkers.push_back(attack);

			hurt = false;
		}

		if (health <= 0.0f)
			alive = false;

		attackTimer.Update(a_deltaTime);
		fireTimer.Update(a_deltaTime);
		knockbackTimer.Update(a_deltaTime);
		knockBackDurationTimer.Update(a_deltaTime);

		healthBar.UpdateHealth(health);
	}
}

void Player::Draw(Camera* a_camera, int a_shader) const
{
	if (alive)
		sprite.DrawAt(a_camera, a_shader, position, size, rotation, transparency);
}

void Player::DrawHealth(Camera* a_camera, int a_shader) const
{
	if (alive)
		healthBar.Draw(a_shader, GetScreenPosition(a_camera) - glm::vec2(0, size.y * 0.8f), glm::vec2(size.x, size.x / 5.0f));
}

void Player::DamagePlayer(DamageType a_damageType, float a_damage)
{
	if (a_damageType == FIRE_DAMAGE)
	{
		if (fireTimer.Ready())
		{
			fireTimer.Start(0.25f);
			health -= a_damage;
		}
	}
	else
	{
		health -= a_damage;
		hurt = true;
	}
}

void Player::KnockBack(glm::vec2 a_direction)
{
	if (knockbackTimer.Ready() && knockBackDurationTimer.Ready())
	{
		knockBackDirection = a_direction;
		knockbackTimer.Start(1.0f);
		knockBackDurationTimer.Start(0.2f);
	}
}