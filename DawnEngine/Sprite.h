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
		float m_x;
		float m_y;
		float m_width;
		float m_height;
		GLuint m_vboID;
		GLTexture m_texture;
	};

}