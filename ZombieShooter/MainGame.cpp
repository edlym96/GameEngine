#include "MainGame.h"


MainGame::MainGame() :m_screenWidth(900),
m_screenHeight(600),
m_gameState(GameState::PLAY),
m_currentLevel(0),
m_fps(60.0f),
m_player(nullptr),
m_numHumansKilled(0),
m_numZombiesKilled(0)
{
}


MainGame::~MainGame()
{
	for (int i = 0; i < m_levels.size(); ++i) {
		delete m_levels[i];
	}
}

void MainGame::run() {
	//_levels.push_back(new Level("Levels/level1.txt"));
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
	// Initialise game engine
	DawnEngine::init();

	// Create window
	m_window.create("Zombie Shooter", m_screenWidth, m_screenHeight, 0);

	// Grey background color
	glClearColor(0.7f, 0.7f, 0.7f, 0.1f);

	// Set up the shaders
	initShaders();

	// Initialise sprite batches
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// Initialise sprite font (make sure initialise after opengl and shaders!)
	m_spriteFont = new DawnEngine::SpriteFont("Fonts//chintzy.ttf", 64);

	// Set up the camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2.0f, m_screenHeight/2.0f));
}

void MainGame::initLevel() {
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets);

	m_humans.push_back(m_player);

	// Declare a random number generator for starting positions
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(1, m_levels[m_currentLevel]->getWidth()-2);
	std::uniform_int_distribution<int> randY(1, m_levels[m_currentLevel]->getHeight()-2);


	// Add all the random humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans();++i) {
		m_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine)*TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		m_humans.back()->init(HUMAN_SPEED,pos);
	}

	//Add the zombies
	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); ++i) {
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	// Set up players guns
	const float BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 0.0f, 100, BULLET_SPEED));
	m_player->addGun(new Gun("Shotgun", 30, 20, 40.0f, 20, BULLET_SPEED));
	m_player->addGun(new Gun("MP5", 3, 1, 5.0f, 30, BULLET_SPEED));
}

void MainGame::initShaders() {
	// Compile color shader
	m_textureProgram.compileShaders("../Shaders/ColorShading.vert", "../Shaders/ColorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	DawnEngine::FpsLimiter fpsLimiter;
	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;
	fpsLimiter.setMaxFPS(60.0f);
	const float CAMERA_SCALE = 1.0f / 2.0f;
	m_camera.setScale(CAMERA_SCALE);

	const float MS_PER_SECOND = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;//number of milliseconds for one frame
	const float MAX_DELTA_TIME = 1.0f;

	float prevTicks = SDL_GetTicks(); // get number of milliseconds since program started
	while (m_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
		checkVictory();
		m_inputManager.update();
		processInput();
		int i = 0;
		while (totalDeltaTime > 0.0f && i<MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			totalDeltaTime -= deltaTime;
			++i;
		}
		m_camera.setPosition(m_player->getPosition());
		m_camera.update();
		m_hudCamera.update();
		drawGame();
		m_fps = fpsLimiter.endFrame();
	}
}

void MainGame::updateAgents(float deltaTime) {
	//UPdate all humans
	for (int i = 0; i < m_humans.size(); ++i) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	// update zombies
	for (int i = 0; i < m_zombies.size(); ++i) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	// UPdate zombie collision
	for (int i = 0; i < m_zombies.size(); ++i) {
		//collide with other zombies
		for (int j = i + 1; j < m_zombies.size(); ++j) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}
		//collide with humans
		for (int j = 1; j < m_humans.size();) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				// Add the new zombie
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
				//Delete the human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
			else {
				++j;
			}
		}
		//Collide with player
		if (m_zombies[i]->collideWithAgent(m_player)) {
			DawnEngine::fatalError("YOU LOSE");
		}
	}

	//UPdate human collision
	for (int i = 0; i < m_humans.size(); ++i) {
		// COllide with other humans
		for (int j = i + 1; j < m_humans.size(); ++j) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}

	}
}

void MainGame::updateBullets(float deltaTime) {
	// Update and collide with world
	for (int i = 0; i < m_bullets.size(); ) {
		// If update returned true, bullet has collided with a wall
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(),deltaTime)) {
			// remove the bullet
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			++i;
		}
	}

	bool wasBulletRemoved;

	// COllide with humans and zombies
	for (int i = 0; i < m_bullets.size();++i) {
		wasBulletRemoved = false;
		for (int j = 0; j < m_zombies.size();) {
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				// Damage zombie, and kill if out of health
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
					//if zombie died remove
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				}
				else {
					++j;
				}
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
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
			for (int j = 1; j < m_humans.size();) {
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						m_numHumansKilled++;
					}
					else {
						++j;
					}
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
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
	if (m_zombies.empty()) {

		std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d humans remaining", m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_currentLevel]->getNumHumans());
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
				m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				m_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				m_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				m_inputManager.releaseKey(evnt.button.button);
				break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0); //Tells GL to use texture unit 0 (OPTIONAL)

	// make sure the shader uses texture 0
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	m_levels[m_currentLevel]->draw();

	// Begin drawing agents
	m_agentSpriteBatch.begin();

	glm::vec2 AGENT_DIMS(AGENT_RADIUS * 2.0f);

	//Draw the humans
	for (int i = 0; i < m_humans.size(); ++i) {
		if (m_camera.isObjectInView(m_humans[i]->getPosition(), AGENT_DIMS)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}

	//Draw the zombies
	for (int i = 0; i < m_zombies.size(); ++i) {
		if (m_camera.isObjectInView(m_zombies[i]->getPosition(), AGENT_DIMS)) {
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	// Draw the bullets
	for (int i = 0; i < m_bullets.size(); ++i) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();
	drawHUD();
	m_textureProgram.unuse();

	// Swap our buffer and draw eveything to the screen
	m_window.swapBuffer();
}

void MainGame::drawHUD() {
	char buffer[256];
	// grab the camera matrix
	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();
	// display text for humans
	sprintf_s(buffer, "Num Humans: %d", m_humans.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0), glm::vec2(0.5), 0.0f, DawnEngine::ColorRGBA8(255, 255, 255, 255));

	// display text for zombies
	sprintf_s(buffer, "Num Zombies: %d", m_zombies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 36), glm::vec2(0.5), 0.0f, DawnEngine::ColorRGBA8(255, 255, 255, 255));
	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}