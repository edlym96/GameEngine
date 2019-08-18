#pragma once

#include <SDL/SDL.h>
#include <DawnEngine/DawnEngine.h>
#include <DawnEngine/Window.h>
#include <DawnEngine/GLSLProgram.h>
#include <DawnEngine/InputManager.h>
#include <DawnEngine/Camera2D.h>
#include <DawnEngine/Timing.h>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/SpriteFont.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include "Level.h"
#include "Player.h"
#include "Zombie.h"
#include "Gun.h"
#include "Bullet.h"
#include <DawnEngine/Errors.h>

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 4.0f;

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	//runs the game
	void run();

private:
	//Initialise core systems
	void initSystems();

	// Initialises the level and sets up everything
	void initLevel();

	//Initialise shaders
	void initShaders();

	// Main Game loop for the program
	void gameLoop();

	//Update all agents
	void updateAgents(float deltaTime);

	//Update all bullets
	void updateBullets(float deltaTime);

	// Check victory condition
	void checkVictory();

	//Handles input processing
	void processInput();

	// Renders the game
	void drawGame();

	// Draws the HUD
	void drawHUD();

	//Member variables
	DawnEngine::Window m_window; //Game window

	DawnEngine::GLSLProgram m_textureProgram; // Shader program

	DawnEngine::InputManager m_inputManager; // Handles input

	DawnEngine::Camera2D m_camera; // Main camera 

	DawnEngine::Camera2D m_hudCamera; // HUD Camera

	DawnEngine::SpriteBatch m_agentSpriteBatch; //Draws all agents

	DawnEngine::SpriteBatch m_hudSpriteBatch; // Sprite batch for drawing fonts

	DawnEngine::SpriteFont* m_spriteFont;

	std::vector<Level*> m_levels; //vector of all levels

	int m_screenWidth = 900;
	int	m_screenHeight = 600;

	Player* m_player;
	std::vector<Human*> m_humans; //Vector for all humans
	std::vector<Zombie*> m_zombies; //Vector for all zombies
	std::vector<Bullet> m_bullets; //Vector for all bullets

	int m_numHumansKilled;
	int m_numZombiesKilled;

	GameState m_gameState;

	int m_currentLevel;

	float m_fps;
};

