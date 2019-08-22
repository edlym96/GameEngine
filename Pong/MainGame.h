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
#include <DawnEngine/ResourceManager.h>

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();
	void run();

private:
	

	//Initialise core systems
	void initSystems();

	// Initialises the game
	void initGame();

	//Initialise shaders
	void initShaders();

	// Main Game loop for the program
	void gameLoop();

	//Handles input processing
	void processInput();

	// Renders the game
	void drawGame();

	//Member variables
	int m_screenWidth;
	int m_screenHeight;

	DawnEngine::Window m_window; //Game window

	DawnEngine::GLSLProgram m_textureProgram; // Shader program

	DawnEngine::InputManager m_inputManager; // Handles input

	DawnEngine::Camera2D m_camera; // Main camera 

	DawnEngine::SpriteBatch m_spriteBatch; //Draws all agents
	
	GameState m_gameState;

	int m_fps;
};

