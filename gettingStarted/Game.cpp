#include "Game.h"


Game::Game()
{
	_window = nullptr;
	_screenWidth = 960;
	_screenHeight = 600;
	_gameState = GameState::PLAY;
}


Game::~Game()
{
}

void Game::run() {
	initSystems();
	gameLoop();
}

void Game::initSystems() {
	/*Initialsie SDL*/
	SDL_Init(SDL_INIT_EVERYTHING);

	//Create Window
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

}

void Game::processInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
		}
	}
}

void Game::gameLoop() {
	while (_gameState != GameState::EXIT) {
		processInput();
	}
}