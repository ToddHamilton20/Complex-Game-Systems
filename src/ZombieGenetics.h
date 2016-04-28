#pragma once

#include "Genetics.h"

#define SPECIAL_TRAIT_COUNT 8

enum
{
	ZOMBIE_BIT_TRAIT_FIRETRAIL = 1,
	ZOMBIE_BIT_TRAIT_ACIDBREATH = 2,
	ZOMBIE_BIT_TRAIT_CHARGE = 4,
	ZOMBIE_BIT_TRAIT_TELEPORT = 8,
	ZOMBIE_BIT_TRAIT_SPLITTING = 16,
	ZOMBIE_BIT_TRAIT_HEALING = 32,
	ZOMBIE_BIT_TRAIT_TRANSPARENT = 64,
	ZOMBIE_BIT_TRAIT_SHOCKWAVE = 128,
	ZOMBIE_BIT_TRAIT_ALL = 255
};

class Zombie;

class ZombieGenetics : public Genetics<Zombie*>
{
public:
	ZombieGenetics() : Genetics(1), startWave(false) {}

	virtual Chromosome<Zombie*> GetMostFit();
	virtual bool TerminationReached();
	void UpdateIMGUI();

	std::vector<Zombie*> zombies;
	std::atomic<bool> startWave;
	std::atomic<bool> zombiesDead;

protected:
	virtual void Populate(std::mt19937& a_random);
	virtual void Fitness(std::mt19937& a_random);
	virtual void Select(std::mt19937& a_random);
	virtual void Crossover(std::mt19937& a_random);
	virtual void Mutate(std::mt19937& a_random);
	virtual void Discard(std::mt19937& a_random);
};