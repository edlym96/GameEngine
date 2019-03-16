#pragma once

#include <GL/glew.h>

struct Vertex {
	struct Position {
		float x;
		float y;
	} position;
	
	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	} color;
};