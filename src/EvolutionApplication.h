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
	Camera* camera;
	Camera* miniMapCamera;

	ZombieGenetics zombieGenetics;

	Sprite grassSprite, dirtSprite, zombieSprite, swipeSprite, fireSprite, projectileSprite, healSprite;
	Sprite healthBarBackSprite, healthBarSprite;
	GameObjects gameObjects;
	MiniMap miniMap;

	bool waveRunning;

	void StartWave();

	template<typename T>
	void DeleteGameObject(std::vector<T*>& a_vector, T* a_item)
	{
		a_vector.erase(std::remove(a_vector.begin(), a_vector.end(), a_item), a_vector.end());
		delete a_item;
	}

	template<typename T>
	void DeleteVector(std::vector<T*>& a_vector)
	{
		for (int i = a_vector.size() - 1; i >= 0; i--)
		{
			delete a_vector[i];
		}

		a_vector.clear();
	}
};