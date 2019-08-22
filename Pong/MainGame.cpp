#include "MainGame.h"



MainGame::MainGame():m_screenWidth(420),m_screenHeight(320), m_gameState(GameState::PLAY), m_fps(0)
{
}


MainGame::~MainGame()
{
}

void MainGame::run() {
	initSystems();
	gameLoop();
}

void MainGame::initSystems() {
	DawnEngine::init();

	m_window.create("PONG", m_screenWidth, m_screenHeight, 0);

	initShaders();

	m_spriteBatch.init();

	m_camera.init(m_screenWidth, m_screenHeight);

	m_camera.setPosition(glm::vec2(m_screenWidth/2,m_screenHeight/2));
}

void MainGame::initShaders() {
	// Compile color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	static DawnEngine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);
	while (m_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();
		processInput();
		m_camera.update();
		drawGame();
		m_fps = fpsLimiter.endFrame();
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
	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch.begin();
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static DawnEngine::GLTexture texture = DawnEngine::ResourceManager::getTexture("Textures/circle.png");
	DawnEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	m_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();

	// Swap our buffer and draw eveything to the screen
	m_window.swapBuffer();
}