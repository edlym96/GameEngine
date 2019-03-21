#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include <DawnEngine/DawnEngine.h>
#include <DawnEngine/Errors.h>
#include <DawnEngine/Sprite.h>
#include <DawnEngine/GLSLProgram.h>
#include <DawnEngine/GLTexture.h>
#include <DawnEngine/Window.h>
#include <DawnEngine/Camera2D.h>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/ResourceManager.h>
#include <DawnEngine/InputManager.h>
#include <DawnEngine/Timing.h>
#include "Bullet.h"
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

	DawnEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	//std::vector<DawnEngine::Sprite*> _sprites;

	DawnEngine::GLSLProgram _colorProgram;
	DawnEngine::Camera2D _camera;

	DawnEngine::SpriteBatch _spriteBatch;

	DawnEngine::InputManager _inputManager;
	DawnEngine::FpsLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	float _fps;
	float _maxFPS;

	float _time;
};

