// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		GameObjects struct, Once instance stores all game objects.

#pragma once

#include <vector>

class Player;

#include "Player.h"
#include "Attack.h"
#include "Zombie.h"
#include "ArenaTile.h"

struct GameObjects
{
	Player* player;
	std::vector<Zombie*> zombies;
	std::vector<Attack*> attacks;
	std::vector<Attack*> fires;
	std::vector<Attack*> projectiles;
	std::vector<Attack*> heals;
	std::vector<ArenaTile*> arenaTiles;
};