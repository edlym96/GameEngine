#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

enum class GameState {PLAY,EXIT};

class Game
{
public:
	Game();
	~Game();

	void run();


private:
	void initSystems(); // Initialise the systems, openGL, SDL, etc.
	void processInput();
	void gameLoop();

	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
};

