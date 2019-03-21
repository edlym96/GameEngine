#pragma once
#include <GL/glew.h>
#include "Vertex.h"
#include <cstddef>
#include "GLTexture.h"
#include <string>
#include "ResourceManager.h"
namespace DawnEngine {
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		//ID that reference Vertex Buffer Object (VBO)
		void draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;
		GLuint _vboID;
		GLTexture _texture;
	};

}