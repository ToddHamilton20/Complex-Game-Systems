#include "HealthBar.h"

void HealthBar::Init(float a_maxHealth, float a_health)
{
	maxHealth = a_maxHealth;

	if (a_health == -1.0f)
		health = maxHealth;
	else
		health = a_health;
}

void HealthBar::UpdateHealth(float a_health)
{
	health = a_health;
}

void HealthBar::Draw(int a_shader, glm::vec2 a_position, glm::vec2 a_size) const
{
	float ratio = health / maxHealth;
	backSprite.DrawAtScreen(a_shader, a_position, a_size);
	sprite.DrawAtScreen(a_shader, a_position - glm::vec2(a_size.x / 2.0f, 0) + glm::vec2((ratio * a_size.x) / 2.0f, 0), a_size * glm::vec2(ratio, 1.0f));
}