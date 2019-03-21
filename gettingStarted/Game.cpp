#include "Game.h"

Game::Game():_screenWidth(960),
	_screenHeight(600), 
	_gameState(GameState::PLAY),
	_time(0.0f),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}


Game::~Game()
{
}

void Game::run() {
	initSystems();

	//_sprites.push_back(new DawnEngine::Sprite());
	//_sprites.back()->init(0.0f, 0.0f, _screenWidth/2, _screenHeight/2, "../Textures/PNG/CharacterRight_Standing.png");
	
	//_sprites.push_back(new DawnEngine::Sprite());
	//_sprites.back()->init(_screenWidth / 2, 0.0f, _screenWidth / 2, _screenHeight / 2, "../Textures/PNG/CharacterRight_Standing.png");

	//_sprites.push_back(new DawnEngine::Sprite());
	//_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "../Textures/PNG/CharacterRight_Standing.png");
	//_playerTexture = ImageLoader::loadPNG("../Textures/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void Game::initSystems() {
	
	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
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

	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
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
	if (_inputManager.isKeyPressed(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED*1.0));
	}
	if (_inputManager.isKeyPressed(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED*-1.0));
	}
	if (_inputManager.isKeyPressed(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED*-1.0, 0.0));
	}
	if (_inputManager.isKeyPressed(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED*1.0, 0.0));
	}
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		//std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 1.00f, 1000);
	}

}

void Game::gameLoop() {
	while (_gameState != GameState::EXIT) {
		// USed for frame time measuring
		_fpsLimiter.beginFrame();

		processInput();
		_time += 0.01;

		_camera.update();

		//update all bullets
		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update()) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		drawGame();

		_fps = _fpsLimiter.endFrame();


		//print once every ten frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

void Game::drawGame() {

	glClearDepth(1.0); // Set a variable for OpenGL to clear to
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen (buffer)
	
	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0); // active texture 0. Could have multiple active texture at once
	
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0); // bind GLTexture0 to textureLocation

	// Set the uniform (constantly changing, uniform throughout the program) time vairable
	//GLint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);

	// Set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	// Upload the uniform to the GPU using glUniform calls
	glUniformMatrix4fv(pLocation, 1,GL_FALSE, &cameraMatrix[0][0]); //address of first element in a 2d matrix

	//for (int i = 0; i < _sprites.size(); i++) {
	//	_sprites[i]->draw();
	//}
	
	_spriteBatch.begin();
	// Draw between begin and end

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static DawnEngine::GLTexture texture = DawnEngine::ResourceManager::getTexture("../Textures/PNG/CharacterRight_Standing.png");
	DawnEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	for (int i = 0; i < _bullets.size(); ++i) {
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	_window.swapBuffer();
}
