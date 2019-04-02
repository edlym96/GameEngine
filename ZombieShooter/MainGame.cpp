#include "MainGame.h"

// GO TO MINUTE 32
// DEBUG TEXTURES FOR LEVEL

MainGame::MainGame() :_screenWidth(900),
_screenHeight(600),
_gameState(GameState::PLAY),
_currentLevel(0),
_fps(60.0f),
_player(nullptr)
{
}


MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); ++i) {
		delete _levels[i];
	}
}

void MainGame::run() {
	//_levels.push_back(new Level("Levels/level1.txt"));
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
	DawnEngine::init();

	_window.create("Zombie Shooter", _screenWidth, _screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 0.1f);
	initShaders();

	_agentSpriteBatch.init();

	_camera.init(_screenWidth, _screenHeight);
}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(4.0f, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager);

	_humans.push_back(_player);
}

void MainGame::initShaders() {
	// Compile color shader
	_textureProgram.compileShaders("../Shaders/ColorShading.vert", "../Shaders/ColorShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	DawnEngine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	while (_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();
		processInput();
		updateAgents();
		_camera.setPosition(_player->getPosition());
		_camera.update();
		drawGame();
		_fps = fpsLimiter.endFrame();
	}
}

void MainGame::updateAgents() {
	for (int i = 0; i < _humans.size(); ++i) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	// dont forget to update zombies
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT:
				break;
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	// make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	_levels[_currentLevel]->draw();

	// Begin drawing agents
	_agentSpriteBatch.begin();

	//Draw the humans
	for (int i = 0; i < _humans.size(); ++i) {
		_humans[i]->draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();
	_textureProgram.unuse();

	// Swap our buffer and draw eveything to the screen
	_window.swapBuffer();
}