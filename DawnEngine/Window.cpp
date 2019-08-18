#include "Window.h"

namespace DawnEngine {
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//Open an SDL Window
		m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if (m_sdlWindow == nullptr) {
			fatalError("SDL Window could not be opened");
		}

		// Give window SDL_GLcontext
		SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created");
		}

		// Initialise GLEW
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialise GLEW");
		}

		// Check openGL version
		std::printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Whenever you clear color buffer, it is cleared to this color

		// Turn off/on vsync
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(m_sdlWindow);
	}

	int Window::getScreenWidth() { return m_screenWidth; }

	int Window::getScreenHeight() { return m_screenHeight; }
}