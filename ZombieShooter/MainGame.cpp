#include "MainGame.h"


MainGame::MainGame() :_screenWidth(900),
_screenHeight(600),
_gameState(GameState::PLAY),
_currentLevel(0),
_fps(60.0f),
_player(nullptr),
_numHumansKilled(0),
_numZombiesKilled(0)
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
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);

	// Declare a random number generator for starting positions
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(1, _levels[_currentLevel]->getWidth()-2);
	std::uniform_int_distribution<int> randY(1, _levels[_currentLevel]->getHeight()-2);


	// Add all the random humans
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans();++i) {
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine)*TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED,pos);
	}

	//Add the zombies
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); ++i) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	// Set up players guns
	const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 0.0f, 100, BULLET_SPEED));
	_player->addGun(new Gun("Shotgun", 30, 20, 40.0f, 20, BULLET_SPEED));
	_player->addGun(new Gun("MP5", 3, 1, 5.0f, 30, BULLET_SPEED));
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
	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;
	fpsLimiter.setMaxFPS(60.0f);
	const float CAMERA_SCALE = 1.0f / 2.0f;
	_camera.setScale(CAMERA_SCALE);

	const float MS_PER_SECOND = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;//number of milliseconds for one frame
	const float MAX_DELTA_TIME = 1.0f;

	float prevTicks = SDL_GetTicks(); // get number of milliseconds since program started
	while (_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
		checkVictory();
		_inputManager.update();
		processInput();
		int i = 0;
		while (totalDeltaTime > 0.0f && i<MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			totalDeltaTime -= deltaTime;
			++i;
		}
		_camera.setPosition(_player->getPosition());
		_camera.update();
		drawGame();
		_fps = fpsLimiter.endFrame();
	}
}

void MainGame::updateAgents(float deltaTime) {
	//UPdate all humans
	for (int i = 0; i < _humans.size(); ++i) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	// update zombies
	for (int i = 0; i < _zombies.size(); ++i) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	// UPdate zombie collision
	for (int i = 0; i < _zombies.size(); ++i) {
		//collide with other zombies
		for (int j = i + 1; j < _zombies.size(); ++j) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//collide with humans
		for (int j = 1; j < _humans.size();) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				// Add the new zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				//Delete the human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
			else {
				++j;
			}
		}
		//Collide with player
		if (_zombies[i]->collideWithAgent(_player)) {
			DawnEngine::fatalError("YOU LOSE");
		}
	}

	//UPdate human collision
	for (int i = 0; i < _humans.size(); ++i) {
		// COllide with other humans
		for (int j = i + 1; j < _humans.size(); ++j) {
			_humans[i]->collideWithAgent(_humans[j]);
		}

	}
}

void MainGame::updateBullets(float deltaTime) {
	// Update and collide with world
	for (int i = 0; i < _bullets.size(); ) {
		// If update returned true, bullet has collided with a wall
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(),deltaTime)) {
			// remove the bullet
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			++i;
		}
	}

	bool wasBulletRemoved;

	// COllide with humans and zombies
	for (int i = 0; i < _bullets.size();++i) {
		wasBulletRemoved = false;
		for (int j = 0; j < _zombies.size();) {
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				// Damage zombie, and kill if out of health
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					//if zombie died remove
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else {
					++j;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				--i;
				break;
			}
			else {
				++j;
			}
		}
		// Handle collision with humans except player
		if (wasBulletRemoved == false) {
			for (int j = 1; j < _humans.size();) {
				if (_bullets[i].collideWithAgent(_humans[j])) {
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else {
						++j;
					}
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					i--;
					break;
				}
				else {
					++j;
				}
			}
		}
	}
}

void MainGame::checkVictory() {
	if (_zombies.empty()) {

		std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d humans remaining", _numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		DawnEngine::fatalError("");
	}
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
				_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
				break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0); //Tells GL to use texture unit 0 (OPTIONAL)

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

	glm::vec2 AGENT_DIMS(AGENT_RADIUS * 2.0f);

	//Draw the humans
	for (int i = 0; i < _humans.size(); ++i) {
		if (_camera.isObjectInView(_humans[i]->getPosition(), AGENT_DIMS)) {
			_humans[i]->draw(_agentSpriteBatch);
		}
	}

	//Draw the zombies
	for (int i = 0; i < _zombies.size(); ++i) {
		if (_camera.isObjectInView(_zombies[i]->getPosition(), AGENT_DIMS)) {
			_zombies[i]->draw(_agentSpriteBatch);
		}
	}

	// Draw the bullets
	for (int i = 0; i < _bullets.size(); ++i) {
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();
	_textureProgram.unuse();

	// Swap our buffer and draw eveything to the screen
	_window.swapBuffer();
}