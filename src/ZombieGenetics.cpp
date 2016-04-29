#include "ZombieGenetics.h"
#include "Zombie.h"

#include <algorithm>
#include <iostream>

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

Chromosome<Zombie*> ZombieGenetics::GetMostFit()
{
	Chromosome<Zombie*> returnZombie(1);
	returnZombie.genes[0] = nullptr;
	return returnZombie;
}

bool ZombieGenetics::TerminationReached()
{
	return false;
}

void ZombieGenetics::Populate(std::mt19937& a_random)
{
	unsigned int populationSize = PopulationSize();
	unsigned int childCount = ChildCount();

	for (int i = 0; i < populationSize + childCount; i++)
	{
		population[i].genes[0] = zombies[i];

		std::uniform_real_distribution<float> distrubution(1.0f, 10.0f);

		population[i].genes[0]->maxHealthRange = glm::vec2(1, 10);
		population[i].genes[0]->maxHealth = distrubution(a_random);

		population[i].genes[0]->speedRange = glm::vec2(5, 15);
		population[i].genes[0]->speed = distrubution(a_random);

		population[i].genes[0]->strengthRange = glm::vec2(1, 10);
		population[i].genes[0]->strength = distrubution(a_random);

		population[i].genes[0]->scaleRange = glm::vec2(0.75f, 1.25f);
		population[i].genes[0]->scale = std::uniform_real_distribution<float>(0.75f, 1.25f)(a_random);

		population[i].genes[0]->specialTraits = 0;

		population[i].genes[0]->active = i < populationSize;
	}
}

void ZombieGenetics::Fitness(std::mt19937& a_random)
{
	unsigned int populationSize = PopulationSize();
	unsigned int childCount = ChildCount();

	const float damageWeight = 1.0f, timeAliveWeight = 0.25f;

	startWave = true;
	zombiesDead = false;
	while (startWave) {}
	while (!zombiesDead) {}

	for (int i = 0; i < populationSize; i++)
	{
		population[i].fitness = population[i].genes[0]->damageToPlayer + population[i].genes[0]->timeAlive;
	}
}

void ZombieGenetics::Select(std::mt19937& a_random)
{
	unsigned int populationSize = PopulationSize();
	unsigned int childCount = ChildCount();

	int totalChances = 0;

	for (int i = 0; i < populationSize; i++)
	{
		totalChances += (int)(population[i].fitness + 1.0f);
	}

	int parentCount = childCount * 2;

	for (int i = 0; i < parentCount; i++)
	{
		int result = rand() % totalChances;
		for (int j = 0; j < populationSize; j++)
		{
			result -= (int)(population[j].fitness + 1.0f);
			if (result < 0)
			{
				result = j;
				break;
			}
		}

		parents[i] = &population[result];
	}
}

void ZombieGenetics::Crossover(std::mt19937& a_random)
{
	unsigned int childCount = ChildCount();

	const float crossoverChance = 70.0f;

	for (int i = 0; i < childCount; i++)
	{
		Zombie	*childGenes = children[i].genes[0],
				*parent1Genes = parents[i * 2]->genes[0],
				*parent2Genes = parents[i * 2 + 1]->genes[0];

		std::uniform_real_distribution<float> distribution(0.0f, 99.0f);

#pragma region MAX_HEALTH
		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->maxHealthRange.x = parent1Genes->maxHealthRange.x + parent2Genes->maxHealthRange.x / 2.0f;
		else
			childGenes->maxHealthRange.x = parent1Genes->maxHealthRange.x;

		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->maxHealthRange.y = parent1Genes->maxHealthRange.y + parent2Genes->maxHealthRange.y / 2.0f;
		else
			childGenes->maxHealthRange.y = parent1Genes->maxHealthRange.y;
#pragma endregion MAX_HEALTH

#pragma region SIZE
		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->scaleRange.x = parent1Genes->scaleRange.x + parent2Genes->scaleRange.x / 2.0f;
		else
			childGenes->scaleRange.x = parent1Genes->scaleRange.x;

		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->scaleRange.y = parent1Genes->scaleRange.y + parent2Genes->scaleRange.y / 2.0f;
		else
			childGenes->scaleRange.y = parent1Genes->scaleRange.y;
#pragma endregion SIZE

#pragma region SPEED
		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->speedRange.x = parent1Genes->speedRange.x + parent2Genes->speedRange.x / 2.0f;
		else
			childGenes->speedRange.x = parent1Genes->speedRange.x;

		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->speedRange.y = parent1Genes->speedRange.y + parent2Genes->speedRange.y / 2.0f;
		else
			childGenes->speedRange.y = parent1Genes->speedRange.y;
#pragma endregion SPEED

#pragma region STRENGTH
		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->strengthRange.x = parent1Genes->strengthRange.x + parent2Genes->strengthRange.x / 2.0f;
		else
			childGenes->strengthRange.x = parent1Genes->strengthRange.x;

		if (distribution(a_random) - crossoverChance < 0.0f)
			childGenes->strengthRange.y = parent1Genes->strengthRange.y + parent2Genes->strengthRange.y / 2.0f;
		else
			childGenes->strengthRange.y = parent1Genes->strengthRange.y;
#pragma endregion STRENGTH

#pragma region SPECIAL_TRAITS
		std::uniform_int_distribution<int> distribution2(0, 1);

		for (int j = 1; j != 256; j *= 2)
		{
			if (distribution2(a_random))
				childGenes->specialTraits |= parent1Genes->specialTraits & j;
			else
				childGenes->specialTraits |= parent2Genes->specialTraits & j;
		}
#pragma endregion SPECIAL_TRAITS
	}
}

void ZombieGenetics::Mutate(std::mt19937& a_random)
{
	unsigned int childCount = ChildCount();

	const float mutationChance = 10.0f;

	for (int i = 0; i < childCount; i++)
	{
		Zombie	*childGenes = children[i].genes[0];

		std::uniform_real_distribution<float> distribution(0.0f, 99.0f);
		std::uniform_int_distribution<int> distribution01(0, 1);

		for (int j = 0; j < 8; j++)
		{
			int posNeg = distribution01(a_random); if (posNeg == 0) posNeg == -1;

			if (distribution(a_random) - mutationChance < 0.0f)
			{
				switch (j)
				{
				case 0: childGenes->maxHealthRange.x += 3;	break;
				case 1: childGenes->speedRange.x += 10;		break;
				case 2: childGenes->strengthRange.x += 5;	break;
				case 3: childGenes->scaleRange.x += 0.05f * posNeg;	break;
				case 4: childGenes->maxHealthRange.y += 3;	break;
				case 5: childGenes->speedRange.y += 10;		break;
				case 6: childGenes->strengthRange.y += 5;	break;
				case 7: childGenes->scaleRange.y += 0.05f * posNeg;	break;
				}
			}
		}

		childGenes->maxHealthRange.x = glm::min(childGenes->maxHealthRange.x, 1000.0f);
		childGenes->speedRange.x = glm::min(childGenes->speedRange.x, 1000.0f);
		childGenes->strengthRange.x = glm::min(childGenes->strengthRange.x, 1000.0f);
		childGenes->scaleRange.x = glm::min(childGenes->scaleRange.x, 1000.0f);

		childGenes->maxHealthRange.y = glm::min(childGenes->maxHealthRange.y, 1000.0f);
		childGenes->speedRange.y = glm::min(childGenes->speedRange.y, 1000.0f);
		childGenes->strengthRange.y = glm::min(childGenes->strengthRange.y, 1000.0f);
		childGenes->scaleRange.y = glm::min(childGenes->scaleRange.y, 1000.0f);

		childGenes->maxHealthRange.x = glm::min(childGenes->maxHealthRange.x, childGenes->maxHealthRange.y);
		childGenes->speedRange.x = glm::min(childGenes->speedRange.x, childGenes->speedRange.y);
		childGenes->strengthRange.x = glm::min(childGenes->strengthRange.x, childGenes->strengthRange.y);
		childGenes->scaleRange.x = glm::min(childGenes->scaleRange.x, childGenes->scaleRange.y);

		childGenes->maxHealth = std::uniform_real_distribution<float>(childGenes->maxHealthRange.x, childGenes->maxHealthRange.y)(a_random);
		childGenes->scale = std::uniform_real_distribution<float>(childGenes->scaleRange.x, childGenes->scaleRange.y)(a_random);
		childGenes->speed = std::uniform_real_distribution<float>(childGenes->speedRange.x, childGenes->speedRange.y)(a_random);
		childGenes->strength = std::uniform_real_distribution<float>(childGenes->strengthRange.x, childGenes->strengthRange.y)(a_random);
		childGenes->maxHealth = childGenes->maxHealth * childGenes->scale;
		childGenes->speed = childGenes->speed * (1 / childGenes->scale);


		for (int j = 1; j < 256; j *= 2)
		{
			if (distribution(a_random) - mutationChance < 0.0f)
				childGenes->specialTraits |= j;
		}
	}
}

struct DiscardSortFunction
{
	inline bool operator()(const Chromosome<Zombie*>& a, const Chromosome<Zombie*>& b)
	{
		return a.fitness < b.fitness;
	}
};

void ZombieGenetics::Discard(std::mt19937& a_random)
{
	unsigned int populationSize = PopulationSize();
	unsigned int childCount = ChildCount();

	for (int i = 0; i < childCount; i++)
	{
		children[i].fitness = 0.0f;
	}

	std::sort(populationPool.begin(), populationPool.end() - childCount, DiscardSortFunction());
	std::swap_ranges(populationPool.begin() + populationSize, populationPool.begin() + populationSize + childCount, populationPool.begin());

	for (int i = 0; i < populationSize; i++)
	{
		population[i].genes[0]->active = true;
	}

	for (int i = 0; i < childCount; i++)
	{
		children[i].genes[0]->active = false;
	}
}

void ZombieGenetics::UpdateIMGUI()
{
	unsigned int populationSize = PopulationSize(), childCount = ChildCount();

	ImVec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 white(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 blue(0.5f, 0.5f, 1.0f, 1.0f);

	ImGui::Begin("Genetic Algorithm");
	ImGui::Columns(2);

	int selectedIndex = -1;

	for (int i = populationSize - 1; i >= 0; i--)
	{
		if (populationSize - i == 1)
		{
			ImGui::SetWindowFontScale(1.5f);
			ImGui::TextColored(blue, "Current Population");
			ImGui::SetWindowFontScale(1.0f);
		}

		if (populationSize - i == populationSize - childCount + 1)
		{
			ImGui::SetWindowFontScale(1.5f);
			ImGui::TextColored(blue, "New Children");
			ImGui::SetWindowFontScale(1.0f);
		}

		std::string name("Population ");
		name.append(std::string(std::to_string(populationSize - i)));
		ImGui::TextColored(white, name.c_str());

		if (ImGui::IsItemHovered())
			selectedIndex = i;
	}

	ImGui::SetWindowFontScale(1.5f);
	ImGui::TextColored(blue, "Discarded Population");
	ImGui::SetWindowFontScale(1.0f);

	for (int i = childCount - 1; i >= 0; i--)
	{
		std::string name("Discarded ");
		name.append(std::string(std::to_string(childCount - i)));
		ImGui::TextColored(white, name.c_str());

		if (ImGui::IsItemHovered())
			selectedIndex = populationSize + i;
	}

	if (selectedIndex != -1)
	{
		Zombie* zombie = population[selectedIndex].genes[0];

		ImGui::NextColumn();
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("");
		ImGui::SetWindowFontScale(1.0f);
		std::string fitness = std::string("Fitness: ").append(std::to_string(population[selectedIndex].fitness));
		ImGui::Text(fitness.c_str());
		ImGui::Text("");
		std::string strength = std::string("Strength: ").append(std::to_string(zombie->strength));
		ImGui::Text(strength.c_str());
		std::string speed = std::string("Speed: ").append(std::to_string(zombie->speed));
		ImGui::Text(speed.c_str());
		std::string size = std::string("Size: ").append(std::to_string(zombie->scale));
		ImGui::Text(size.c_str());
		std::string health = std::string("Max Health: ").append(std::to_string(zombie->maxHealth));
		ImGui::Text(health.c_str());
		ImGui::Text("");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_FIRETRAIL ? green : red, "Fire Trail");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_ACIDBREATH ? green : red, "Acid Breath");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_CHARGE ? green : red, "Charge");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_TELEPORT ? green : red, "Teleport");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_SPLITTING ? green : red, "Splitting");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_HEALING ? green : red, "Healing");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_TRANSPARENT ? green : red, "Invisibility");
		ImGui::TextColored(zombie->specialTraits & ZOMBIE_BIT_TRAIT_SHOCKWAVE ? green : red, "Knockback");
		ImGui::NextColumn();
	}

	ImGui::End();
}