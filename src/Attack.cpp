#include "Attack.h"

#include "GameObjects.h"

void Attack::Update(GameObjects* a_gameObjects, float a_deltaTime)
{
	if (timer != INFINITY)
	{
		if (timer > 0)
			timer -= a_deltaTime;
		if (timer < 0)
			timer = 0;
		if (timer == 0 && alive)
			alive = false;

		if (fade)
			transparency = timer / maxTimer;
	}

	position += velocity * a_deltaTime;
}