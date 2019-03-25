#pragma once

#include <DawnEngine/DawnEngine.h>
#include <DawnEngine/Window.h>
#include <DawnEngine/GLSLProgram.h>
#include <DawnEngine/InputManager.h>
#include <DawnEngine/Camera2D.h>

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

	//Initialise shaders
	void initShaders();

	// Main Game loop for the program
	void gameLoop();

	//Handles input processing
	void processInput();

	// Renders the game
	void drawGame();

	//Member variables
	DawnEngine::Window _window; //Game window

	DawnEngine::GLSLProgram _textureProgram; // Shader program

	DawnEngine::InputManager _inputManager; // Handles input

	DawnEngine::Camera2D _camera; // Main camera
};

