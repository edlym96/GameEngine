#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include "Errors.h"
namespace DawnEngine {
	enum windowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth();

		int getScreenHeight();

	private:
		SDL_Window* m_sdlWindow;
		int m_screenWidth;
		int m_screenHeight;
	};

}