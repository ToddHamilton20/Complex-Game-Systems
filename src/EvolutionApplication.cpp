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

// Comment
bool EvolutionApplication::startup()
{
	Window::GetInstance().InitialiseWindow("Todd Hamilton - Evolution Application", SCREEN_X, SCREEN_Y);

	GLFWwindow* window = glfwGetCurrentContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1280;
	io.DisplaySize.y = 720;

	shader = Shader::CreateShader("Shader.vert", "Shader.frag");
	glUseProgram(shader);

	camera = new Camera(SCREEN_X, SCREEN_Y);
	miniMapCamera = new Camera(ARENA_SIZE + TILE_SIZE * 2.0f, ARENA_SIZE + TILE_SIZE * 2.0f);

	miniMap.Init(glm::vec2(ARENA_SIZE + TILE_SIZE * 2.0f, ARENA_SIZE + TILE_SIZE * 2.0f), glm::vec2(720, 720), glm::vec2(640, 360));

	grassSprite.Init("Grass.png");
	dirtSprite.Init("Dirt.png");
	zombieSprite.Init("Zombie.png");
	swipeSprite.Init("Swipe.png");
	fireSprite.Init("Fire.png");
	projectileSprite.Init("Projectile.png");
	healSprite.Init("Heal.png");
	healthBarSprite.Init("HealthBar.png");
	healthBarBackSprite.Init("HealthBarBack.png");

	gameObjects.player = new Player();
	gameObjects.player->position = glm::vec2(0, 0);
	gameObjects.player->sprite.Init("Player.png");
	gameObjects.player->size = glm::vec2(125, 125);
	gameObjects.player->healthBar.sprite = healthBarSprite;
	gameObjects.player->healthBar.backSprite = healthBarBackSprite;
	gameObjects.player->camera = camera;
	gameObjects.player->swipeSprite = swipeSprite;

	for (int i = 0; i < POPULATION_SIZE + CHILD_COUNT; i++)
	{
		gameObjects.zombies.push_back(new Zombie());
		zombieGenetics.zombies.push_back(gameObjects.zombies[i]);
		gameObjects.zombies[i]->sprite = zombieSprite;
		gameObjects.zombies[i]->projectileSprite = projectileSprite;
		gameObjects.zombies[i]->fireSprite = fireSprite;
		gameObjects.zombies[i]->healSprite = healSprite;
		gameObjects.zombies[i]->healthBar.sprite = healthBarSprite;
		gameObjects.zombies[i]->healthBar.backSprite = healthBarBackSprite;
	}

	for (int x = 0; x < ARENA_SIZE / TILE_SIZE + 2; x++)
	{
		for (int y = 0; y < ARENA_SIZE / TILE_SIZE + 2; y++)
		{
			ArenaTile* tile = new ArenaTile();
			tile->position = glm::vec2(-ARENA_SIZE / 2 - TILE_SIZE / 2 + x * TILE_SIZE, -ARENA_SIZE / 2 - TILE_SIZE / 2 + y * TILE_SIZE);
			tile->size = glm::vec2(TILE_SIZE, TILE_SIZE);

			if (x == 0 || x == ARENA_SIZE / TILE_SIZE + 1 || y == 0 || y == ARENA_SIZE / TILE_SIZE + 1)
				tile->sprite = dirtSprite;
			else
				tile->sprite = grassSprite;

			gameObjects.arenaTiles.push_back(tile);
		}
	}

	zombieGenetics.ThreadedInitialise(POPULATION_SIZE, CHILD_COUNT);
	StartWave();

	Gizmos::create();

	return true;
}

void EvolutionApplication::shutdown()
{
	ImGui_ImplGlfwGL3_Shutdown();

	DeleteVector(gameObjects.zombies);
	DeleteVector(gameObjects.arenaTiles);
	DeleteVector(gameObjects.fires);
	DeleteVector(gameObjects.heals);
	DeleteVector(gameObjects.projectiles);
	DeleteVector(gameObjects.attacks);

	delete gameObjects.player;

	grassSprite.Destroy();
	dirtSprite.Destroy();
	zombieSprite.Destroy();
	fireSprite.Destroy();
	projectileSprite.Destroy();
	swipeSprite.Destroy();
	healSprite.Destroy();
	gameObjects.player->sprite.Destroy();
	healthBarSprite.Destroy();
	healthBarBackSprite.Destroy();

	Gizmos::destroy();

	glDeleteProgram(shader);
	delete camera;
	delete miniMapCamera;
}

bool EvolutionApplication::update(float a_deltaTime)
{
	if (Window::GetInstance().ShouldClose())
		return false;

	ImGui_ImplGlfwGL3_NewFrame();

	Gizmos::clear();

	if (!zombieGenetics.AnyThreadsRunning())
		zombieGenetics.SimulateThreadedGeneration();

	if (zombieGenetics.startWave)
		StartWave();

	gameObjects.player->Update(&gameObjects, a_deltaTime);

	glm::vec2 cameraPos = glm::clamp(gameObjects.player->position,
		glm::vec2(-ARENA_SIZE / 2 - TILE_SIZE + SCREEN_X / 2, -ARENA_SIZE / 2 - TILE_SIZE + SCREEN_Y / 2),
		glm::vec2(ARENA_SIZE / 2 + TILE_SIZE - SCREEN_X / 2, ARENA_SIZE / 2 + TILE_SIZE - SCREEN_Y / 2));

	camera->SetPos(cameraPos);

	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		if (gameObjects.zombies[i]->active && gameObjects.zombies[i]->alive)
			gameObjects.zombies[i]->Update(&gameObjects, a_deltaTime);
	}
	
	for (int i = gameObjects.fires.size() - 1; i >= 0; i--)
	{
		gameObjects.fires[i]->Update(&gameObjects, a_deltaTime);
	
		if (glm::length(gameObjects.player->position - gameObjects.fires[i]->position) < gameObjects.player->size.x / 2.0f + gameObjects.fires[i]->size.x / 2.0f)
		{
			gameObjects.player->DamagePlayer(FIRE_DAMAGE, 1.0f);
			gameObjects.fires[i]->owner->damageToPlayer = gameObjects.fires[i]->owner->damageToPlayer + 1.0f;
		}
	
		if (!gameObjects.fires[i]->alive)
			DeleteGameObject(gameObjects.fires, gameObjects.fires[i]);
	}
	
	for (int i = gameObjects.attacks.size() - 1; i >= 0; i--)
	{
		gameObjects.attacks[i]->Update(&gameObjects, a_deltaTime);
	
		if (!gameObjects.attacks[i]->alive)
			DeleteGameObject(gameObjects.attacks, gameObjects.attacks[i]);
	}
	
	for (int i = gameObjects.heals.size() - 1; i >= 0; i--)
	{
		gameObjects.heals[i]->Update(&gameObjects, a_deltaTime);
	
		if (!gameObjects.heals[i]->alive)
			DeleteGameObject(gameObjects.heals, gameObjects.heals[i]);
	}
	
	for (int i = gameObjects.projectiles.size() - 1; i >= 0; i--)
	{
		bool dealtDamage = false;
	
		gameObjects.projectiles[i]->Update(&gameObjects, a_deltaTime);
	
		if (glm::length(gameObjects.player->position - gameObjects.projectiles[i]->position) < gameObjects.player->size.x / 2.0f + gameObjects.projectiles[i]->size.x / 2.0f)
		{
			gameObjects.player->DamagePlayer(PROJECTILE_DAMAGE, 5.0f);
			gameObjects.projectiles[i]->owner->damageToPlayer = gameObjects.projectiles[i]->owner->damageToPlayer + 5.0f;
			dealtDamage = true;
		}
	
		if (!gameObjects.projectiles[i]->alive || dealtDamage)
			DeleteGameObject(gameObjects.projectiles, gameObjects.projectiles[i]);
	}

	for (int i = gameObjects.zombies.size() - 1; i >= POPULATION_SIZE + CHILD_COUNT; i--)
	{
		if (!gameObjects.zombies[i]->alive)
			DeleteGameObject(gameObjects.zombies, gameObjects.zombies[i]);
	}

	if (waveRunning && gameObjects.zombies.size() == POPULATION_SIZE + CHILD_COUNT)
	{
		bool zombiesDead = true;
		for (int i = 0; i < gameObjects.zombies.size(); i++)
		{
			if (gameObjects.zombies[i]->alive && gameObjects.zombies[i]->active)
			{
				zombiesDead = false;
				break;
			}
		}

		if (zombiesDead)
		{
			DeleteVector(gameObjects.fires);
			DeleteVector(gameObjects.attacks);
			DeleteVector(gameObjects.heals);
			DeleteVector(gameObjects.projectiles);

			waveRunning = false;
			zombieGenetics.zombiesDead = true;
		}
	}

	zombieGenetics.UpdateIMGUI();

	return true;
}

void EvolutionApplication::draw()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < gameObjects.arenaTiles.size(); i++)
	{
		gameObjects.arenaTiles[i]->Draw(camera, shader);
	}

	for (int i = 0; i < gameObjects.fires.size(); i++)
	{
		gameObjects.fires[i]->Draw(camera, shader);
	}
	
	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		gameObjects.zombies[i]->Draw(camera, shader);
	}
	
	for (int i = 0; i < gameObjects.projectiles.size(); i++)
	{
		gameObjects.projectiles[i]->Draw(camera, shader);
	}
	
	for (int i = 0; i < gameObjects.heals.size(); i++)
	{
		gameObjects.heals[i]->Draw(camera, shader);
	}
	
	gameObjects.player->Draw(camera, shader);
	
	for (int i = 0; i < gameObjects.attacks.size(); i++)
	{
		gameObjects.attacks[i]->Draw(camera, shader);
	}

	gameObjects.player->DrawHealth(camera, shader);

	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		gameObjects.zombies[i]->DrawHealth(camera, shader);
	}

	if (Window::GetInstance().GetKey(GLFW_KEY_TAB))
		miniMap.Draw(gameObjects, miniMapCamera, shader, shader);

	glDisable(GL_BLEND);

	Gizmos::draw(camera->getProjectionView());

	ImGui::Render();
}

void EvolutionApplication::StartWave()
{
	for (int i = 0; i < gameObjects.zombies.size(); i++)
	{
		glm::vec2 position(rand() % 6000 - 3000, rand() % 6000 - 3000);
		while (glm::length(gameObjects.player->position - position) < 250.0f)
		{
			position.x = rand() % 6000 - 3000;
			position.y = rand() % 6000 - 3000;
		}

		gameObjects.zombies[i]->position = position;
		gameObjects.zombies[i]->size = glm::vec2(100, 100) * (float)gameObjects.zombies[i]->scale;
		gameObjects.zombies[i]->Respawn();
	}

	gameObjects.player->Init(750.0f, 7.0f, 100.0f, 100.0f);
	waveRunning = true;

	zombieGenetics.startWave = false;
}