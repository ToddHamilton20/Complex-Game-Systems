#include "EvolutionApplication.h"

#include "gl_core_4_4.h"
#include "Shader.h"
#include "Camera.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "GLFW\glfw3.h"
#include "Window.h"

#include <iostream>
#include <algorithm>

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include "Defines.h"

bool EvolutionApplication::Startup()
{
	Window::GetInstance().InitialiseWindow("Todd Hamilton - Evolution Application", SCREEN_X, SCREEN_Y);

	GLFWwindow* window = glfwGetCurrentContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = SCREEN_X;
	io.DisplaySize.y = SCREEN_Y;

	shader = Shader::CreateShader("Shader.vert", "Shader.frag");
	glUseProgram(shader);

	camera = new Camera(SCREEN_X, SCREEN_Y);
	miniMapCamera = new Camera(ARENA_SIZE + TILE_SIZE * 2.0f, ARENA_SIZE + TILE_SIZE * 2.0f);

	miniMap.Init(glm::vec2(ARENA_SIZE + TILE_SIZE * 2.0f, ARENA_SIZE + TILE_SIZE * 2.0f), glm::vec2(SCREEN_Y, SCREEN_Y), glm::vec2(SCREEN_X / 2.0f, SCREEN_Y / 2.0f));

	// Init textures
	Sprites& instance = Sprites::GetInstance();
	instance.InitSprite("Grass", "Grass.png");
	instance.InitSprite("Dirt", "Dirt.png");
	instance.InitSprite("Player", "Player.png");
	instance.InitSprite("Zombie", "Zombie.png");
	instance.InitSprite("Swipe", "Swipe.png");
	instance.InitSprite("Fire", "Fire.png");
	instance.InitSprite("Projectile", "Projectile.png");
	instance.InitSprite("Heal", "Heal.png");
	instance.InitSprite("HealthBar", "HealthBar.png");
	instance.InitSprite("HealthBarBack", "HealthBarBack.png");
	instance.InitSprite("HitMarker", "HitMarker.png");
	instance.InitSprite("FireHitMarker", "FireHitMarker.png");

	// Create and init player
	gameObjects.player = new Player();
	gameObjects.player->position = glm::vec2(0, 0);
	gameObjects.player->size = glm::vec2(125, 125);
	gameObjects.player->healthBar.backSprite = healthBarBackSprite;
	gameObjects.player->camera = camera;
	gameObjects.player->swipeSprite = swipeSprite;
	gameObjects.player->hitMarkerSprite = hitMarkerSprite;
	
	// Structure of zombie vector:
	// Index 0 to POPULATION_SIZE - 1:
	//		General population.
	// Index POPULATION_SIZE to POPULATION_SIZE + CHILD_COUNT - 1:
	//		Discarded population / Next children
	// Index POPULATION_SIZE + CHILD_COUNT to vetor.size():
	//		Splitter zombies

	// Create and init all REGULAR zombies (Population and Children)
	for (int i = 0; i < POPULATION_SIZE + CHILD_COUNT; i++)
	{
		gameObjects.zombies.push_back(new Zombie());
		gameObjects.zombies[i]->projectileSprite = projectileSprite;
		gameObjects.zombies[i]->fireSprite = fireSprite;
		gameObjects.zombies[i]->healSprite = healSprite;

		// Add the zombies to the genetic algorithm
		zombieGenetics.zombies.push_back(gameObjects.zombies[i]);
	}

	// Create arena tiles
	for (int x = 0; x < ARENA_SIZE / TILE_SIZE + 2; x++)
	{
		for (int y = 0; y < ARENA_SIZE / TILE_SIZE + 2; y++)
		{
			ArenaTile* tile = new ArenaTile();
			tile->position = glm::vec2(-ARENA_SIZE / 2 - TILE_SIZE / 2 + x * TILE_SIZE, -ARENA_SIZE / 2 - TILE_SIZE / 2 + y * TILE_SIZE);
			tile->size = glm::vec2(TILE_SIZE, TILE_SIZE);

			// Outside border sprites are dirt
			if (x == 0 || x == ARENA_SIZE / TILE_SIZE + 1 || y == 0 || y == ARENA_SIZE / TILE_SIZE + 1)
				tile->sprite = dirtSprite;
			else
				tile->sprite = grassSprite;

			gameObjects.arenaTiles.push_back(tile);
		}
	}

	// Initialise genetic algorithm
	zombieGenetics.ThreadedInitialise(POPULATION_SIZE, CHILD_COUNT);

	Gizmos::create();

	return true;
}

void EvolutionApplication::Shutdown()
{
	ImGui_ImplGlfwGL3_Shutdown();

	// Delete all allocated memory
	DeleteVector(gameObjects.zombies);
	DeleteVector(gameObjects.arenaTiles);
	DeleteVector(gameObjects.fires);
	DeleteVector(gameObjects.heals);
	DeleteVector(gameObjects.projectiles);
	DeleteVector(gameObjects.attacks);

	delete gameObjects.player;

	Sprites::GetInstance().DestroySprites();

	miniMap.Destroy();
	Gizmos::destroy();

	glDeleteProgram(shader);
	delete camera;
	delete miniMapCamera;
}

bool EvolutionApplication::Update(float a_deltaTime)
{
	if (Window::GetInstance().ShouldClose())
		return false;

	ImGui_ImplGlfwGL3_NewFrame();

	Gizmos::clear();

	// If genetic algorithm isn't creating a new generation, create one.
	if (!zombieGenetics.AnyThreadsRunning())
		zombieGenetics.SimulateThreadedGeneration();

	// If genetic algorithm is ready (i.e. has completed reproduction), start the wave.
	if (zombieGenetics.startWave)
		StartWave();

	gameObjects.player->Update(&gameObjects, a_deltaTime);

	// Clamp camera to the arena
	glm::vec2 cameraPos = glm::clamp(gameObjects.player->position,
		glm::vec2(-ARENA_SIZE / 2 - TILE_SIZE + SCREEN_X / 2, -ARENA_SIZE / 2 - TILE_SIZE + SCREEN_Y / 2),
		glm::vec2(ARENA_SIZE / 2 + TILE_SIZE - SCREEN_X / 2, ARENA_SIZE / 2 + TILE_SIZE - SCREEN_Y / 2));

	camera->SetPos(cameraPos);

	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		if (gameObjects.zombies[i]->active && gameObjects.zombies[i]->alive)
			gameObjects.zombies[i]->Update(&gameObjects, a_deltaTime);

		gameObjects.zombies[i]->specialTraits = ZOMBIE_BIT_TRAIT_FIRETRAIL;
	}
	
	// Update fires
	for (int i = gameObjects.fires.size() - 1; i >= 0; i--)
	{
		gameObjects.fires[i]->Update(&gameObjects, a_deltaTime);
	
		// If player touches fire, damage the player. Fire damage type must be used so the player isn't damaged by multiple fires at the same time.
		if (glm::length(gameObjects.player->position - gameObjects.fires[i]->position) < gameObjects.player->size.x / 2.0f + gameObjects.fires[i]->size.x / 2.0f)
		{
			gameObjects.player->DamagePlayer(FIRE_DAMAGE, 5.0f);
			gameObjects.fires[i]->owner->damageToPlayer = gameObjects.fires[i]->owner->damageToPlayer + 5.0f;

			Attack* attack = new Attack("FireHitMarker", 0.5f, glm::vec2(0, 0), true);
			attack->position = glm::vec2(SCREEN_X / 2.0f, SCREEN_Y / 2.0f);
			attack->size = glm::vec2(SCREEN_X, SCREEN_Y);
			gameObjects.hitMarkers.push_back(attack);
		}
	
		if (!gameObjects.fires[i]->alive)
			DeleteGameObject(gameObjects.fires, gameObjects.fires[i]);
	}
	
	// Update attacks
	for (int i = gameObjects.attacks.size() - 1; i >= 0; i--)
	{
		gameObjects.attacks[i]->Update(&gameObjects, a_deltaTime);
	
		if (!gameObjects.attacks[i]->alive)
			DeleteGameObject(gameObjects.attacks, gameObjects.attacks[i]);
	}
	
	// Update heals
	for (int i = gameObjects.heals.size() - 1; i >= 0; i--)
	{
		gameObjects.heals[i]->Update(&gameObjects, a_deltaTime);
	
		if (!gameObjects.heals[i]->alive)
			DeleteGameObject(gameObjects.heals, gameObjects.heals[i]);
	}
	
	// Update projectiles.
	for (int i = gameObjects.projectiles.size() - 1; i >= 0; i--)
	{
		bool dealtDamage = false;
	
		gameObjects.projectiles[i]->Update(&gameObjects, a_deltaTime);
	
		// If projectile touches player, damage the player
		if (glm::length(gameObjects.player->position - gameObjects.projectiles[i]->position) < gameObjects.player->size.x / 2.0f + gameObjects.projectiles[i]->size.x / 2.0f)
		{
			gameObjects.player->DamagePlayer(ZOMBIE_DAMAGE, 5.0f);
			gameObjects.projectiles[i]->owner->damageToPlayer = gameObjects.projectiles[i]->owner->damageToPlayer + 5.0f;
			dealtDamage = true;
		}
	
		// If hit player or timed out, delete the projectile.
		if (!gameObjects.projectiles[i]->alive || dealtDamage)
			DeleteGameObject(gameObjects.projectiles, gameObjects.projectiles[i]);
	}

	// Update hit markers
	for (int i = gameObjects.hitMarkers.size() - 1; i >= 0; i--)
	{
		gameObjects.hitMarkers[i]->Update(&gameObjects, a_deltaTime);

		if (!gameObjects.hitMarkers[i]->alive)
			DeleteGameObject(gameObjects.hitMarkers, gameObjects.hitMarkers[i]);
	}

	// Delete any SPLITTER zombies that may still be alive.
	for (int i = gameObjects.zombies.size() - 1; i >= POPULATION_SIZE + CHILD_COUNT; i--)
	{
		if (!gameObjects.zombies[i]->alive)
			DeleteGameObject(gameObjects.zombies, gameObjects.zombies[i]);
	}

	// If wave still running and SPLITTER zombies dead
	if (waveRunning && gameObjects.zombies.size() == POPULATION_SIZE + CHILD_COUNT)
	{
		// Check if all zombies are dead
		bool zombiesDead = true;
		for (int i = 0; i < gameObjects.zombies.size(); i++)
		{
			if (gameObjects.zombies[i]->alive && gameObjects.zombies[i]->active)
			{
				zombiesDead = false;
				break;
			}
		}

		// If all zombies are dead
		if (zombiesDead)
		{
			// Clear temp gameObjects
			DeleteVector(gameObjects.fires);
			DeleteVector(gameObjects.attacks);
			DeleteVector(gameObjects.heals);
			DeleteVector(gameObjects.projectiles);

			waveRunning = false;
			// Set zombiesDead flag in genetic algorithm to true. This will allow it to evaluate fitness before the next wave.
			zombieGenetics.zombiesDead = true;
		}
	}

	// Add IMGUI window for genetic algorithm
	zombieGenetics.UpdateIMGUI();

	return true;
}

void EvolutionApplication::Draw()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// NOT CLEARING DEPTH BUFFER, DEPTH BUFFER IS OFF.
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw arena tiles
	for (int i = 0; i < gameObjects.arenaTiles.size(); i++)
	{
		gameObjects.arenaTiles[i]->Draw(camera, shader);
	}

	// Draw fires
	for (int i = 0; i < gameObjects.fires.size(); i++)
	{
		gameObjects.fires[i]->Draw(camera, shader);
	}
	
	// Draw zombies
	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		gameObjects.zombies[i]->Draw(camera, shader);
	}
	
	// Draw projectiles
	for (int i = 0; i < gameObjects.projectiles.size(); i++)
	{
		gameObjects.projectiles[i]->Draw(camera, shader);
	}
	
	// Draw heals
	for (int i = 0; i < gameObjects.heals.size(); i++)
	{
		gameObjects.heals[i]->Draw(camera, shader);
	}
	
	// Draw player
	gameObjects.player->Draw(camera, shader);
	
	// Draw attacks
	for (int i = 0; i < gameObjects.attacks.size(); i++)
	{
		gameObjects.attacks[i]->Draw(camera, shader);
	}

	// Draw health bars
	gameObjects.player->DrawHealth(camera, shader);
	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		gameObjects.zombies[i]->DrawHealth(camera, shader);
	}

	// Draw "mini"map if tab is pressed.
	if (Window::GetInstance().GetKey(GLFW_KEY_TAB))
		miniMap.Draw(gameObjects, miniMapCamera, shader, shader);

	// Draw hit markers
	for (int i = 0; i < gameObjects.hitMarkers.size(); i++)
	{
		gameObjects.hitMarkers[i]->sprite.DrawAtScreen(shader,
			gameObjects.hitMarkers[i]->position,
			gameObjects.hitMarkers[i]->size, 0.0f,
			gameObjects.hitMarkers[i]->transparency);
	}

	glDisable(GL_BLEND);

	Gizmos::draw(camera->GetProjectionView());

	ImGui::Render();
}

void EvolutionApplication::StartWave()
{
	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		glm::vec2 position;
		do
		{
			// Set position to random positions in arena. Not an equal distribution, but close enough.
			position.x = rand() % ARENA_SIZE - ARENA_SIZE / 2.0f;
			position.y = rand() % ARENA_SIZE - ARENA_SIZE / 2.0f;
		}
		// If too close to player (< 250 pixels), get a new position
		while (glm::length(gameObjects.player->position - position) < 250.0f);

		// Respawn zombies
		gameObjects.zombies[i]->position = position;
		gameObjects.zombies[i]->size = glm::vec2(100, 100) * (float)gameObjects.zombies[i]->scale;
		gameObjects.zombies[i]->Respawn();
	}

	// Re Initialise player, just used to reset health bar.
	gameObjects.player->Init(750.0f, 7.0f, 100.0f, 100.0f);

	waveRunning = true;
	zombieGenetics.startWave = false;
}