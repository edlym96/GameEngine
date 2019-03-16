#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Errors.h"
#include "Sprite.h"
#include "GLSLProgram.h"

enum class GameState {PLAY,EXIT};

class Game
{
public:
	Game();
	~Game();

	void run();


private:
	void initSystems(); // Initialise the systems, openGL, SDL, etc.
	void initShaders();
	void processInput();
	void gameLoop();
	void drawGame();

	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	Sprite _sprite;

	GLSLProgram _colorProgram;

	float _time;
};

