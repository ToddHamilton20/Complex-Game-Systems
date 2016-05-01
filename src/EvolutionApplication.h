// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		EvolutionApplication class, game is run here. Includes all game code and objects

#pragma once

#include "BaseApplication.h"

#include "Sprite.h"
#include "Gizmos.h"
#include "ZombieGenetics.h"
#include "GameObjects.h"
#include "MiniMap.h"
#include <vector>

class Camera;

// Main game class
class EvolutionApplication : public BaseApplication
{
public:
	EvolutionApplication() {}
	virtual ~EvolutionApplication() {}

	virtual bool Startup();
	virtual void Shutdown();

	virtual bool Update(float a_deltaTime);
	virtual void Draw();

private:
	unsigned int shader;
	// Camera covering 1280x720 around the player
	Camera* camera;
	// Camera covering the 6000x6000 arena around it's centre.
	Camera* miniMapCamera;

	// Genetic algorithm
	ZombieGenetics zombieGenetics;

	Sprite grassSprite, dirtSprite, zombieSprite, swipeSprite, fireSprite, projectileSprite, healSprite;
	Sprite playerSprite, healthBarBackSprite, healthBarSprite, hitMarkerSprite, fireHitMarkerSprite;
	GameObjects gameObjects;
	MiniMap miniMap;

	bool waveRunning;

	void StartWave();

	template<typename T>
	// Removes an item* by value from a vector and DELETES the item*.
	void DeleteGameObject(std::vector<T*>& a_vector, T* a_item)
	{
		a_vector.erase(std::remove(a_vector.begin(), a_vector.end(), a_item), a_vector.end());
		delete a_item;
	}

	template<typename T>
	// Removes all items* from a vector and DELETES all items*.
	void DeleteVector(std::vector<T*>& a_vector)
	{
		for (int i = a_vector.size() - 1; i >= 0; i--)
		{
			delete a_vector[i];
		}

		a_vector.clear();
	}
};