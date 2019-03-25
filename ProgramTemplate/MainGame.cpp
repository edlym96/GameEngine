#include "MainGame.h"



MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

void MainGame::run() {

}

void MainGame::initSystems() {

}

void MainGame::initShaders() {
	// Compile color shader
	_textureProgram.compileShaders("Shader1", "Shader2");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

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

	//Draw code goes here

	// Swap our buffer and draw eveything to the screen
	_window.swapBuffer();
}