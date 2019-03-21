#include "DawnEngine.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
namespace DawnEngine {
	int init() {
		/*Initialsie SDL*/
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Sets two windows. Instead of having one window where we repeatedly draw and clear, have two windows as one is cleared the other is being drawn

		return 0;
	}
}