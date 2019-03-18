#include "Game.h"

Game::Game():_window(nullptr),
	_screenWidth(960),
	_screenHeight(600), 
	_gameState(GameState::PLAY),
	_time(0.0f)
{
}


Game::~Game()
{
}

void Game::run() {
	initSystems();

	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);

	_playerTexture = ImageLoader::loadPNG("../Textures/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void Game::initSystems() {
	/*Initialsie SDL*/
	SDL_Init(SDL_INIT_EVERYTHING);

	//Create Window
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

	if (_window == nullptr) {
		fatalError("SDL Window could not be opened");
	}

	// Give window SDL_GLcontext
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created");
	}

	// Initialise GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialise GLEW");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Sets two windows. Instead of having one window where we repeatedly draw and clear, have two windows as one is cleared the other is being drawn

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Whenever you clear color buffer, it is cleared to this color

	initShaders();
}

void Game::initShaders() {
	_colorProgram.compileShaders("../Shaders/colorShading.vert", "../Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
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
		_time += 0.01;
		drawGame();
	}
}

void Game::drawGame() {

	glClearDepth(1.0); // Set a variable for OpenGL to clear to
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen (buffer)
	
	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0); // active texture 0. Could have multiple active texture at once
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0); // bind GLTexture0 to textureLocation

	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);
}