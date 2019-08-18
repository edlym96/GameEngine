#pragma once

#include <GL/glew.h>
namespace DawnEngine {
	struct Position {
		float x;
		float y;
	};

	struct ColorRGBA8 {
		ColorRGBA8() {};
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A):r(R),g(G),b(B),a(A) {};
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct Vertex {
		
		Position position;

		struct UV {
			float u;
			float v;
		} uv;
		
		ColorRGBA8 color;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}