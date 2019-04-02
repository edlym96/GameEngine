#pragma once

#include <SDL/SDL.h>
#include <DawnEngine/DawnEngine.h>
#include <DawnEngine/Window.h>
#include <DawnEngine/GLSLProgram.h>
#include <DawnEngine/InputManager.h>
#include <DawnEngine/Camera2D.h>
#include <DawnEngine/Timing.h>
#include <DawnEngine/SpriteBatch.h>
#include <iostream>
#include "Level.h"
#include "Player.h"

#include "Zombie.h"

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
	void updateAgents();

	//Handles input processing
	void processInput();

	// Renders the game
	void drawGame();

	//Member variables
	DawnEngine::Window _window; //Game window

	DawnEngine::GLSLProgram _textureProgram; // Shader program

	DawnEngine::InputManager _inputManager; // Handles input

	DawnEngine::Camera2D _camera; // Main camera 

	DawnEngine::SpriteBatch _agentSpriteBatch; //Draws all agents

	std::vector<Level*> _levels; //vector of all levels

	int _screenWidth, _screenHeight;

	Player* _player;
	std::vector<Human*> _humans; //Vector for all humans
	std::vector<Zombie*> _zombies; //Vector for all zombies

	GameState _gameState;

	int _currentLevel;

	float _fps;
};

