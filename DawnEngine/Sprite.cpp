#include "Sprite.h"
namespace DawnEngine {
	Sprite::Sprite() :m_vboID(0)
	{

	}


	Sprite::~Sprite()
	{
		if (m_vboID != 0) {
			glDeleteBuffers(1, &m_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		m_texture = ResourceManager::getTexture(texturePath);

		//Create buffer it hasnt already been generated
		if (m_vboID == 0) {
			glGenBuffers(1, &m_vboID);
		}

		Vertex vertexData[6];//6 vertices, x and y for each
		vertexData[0].setPosition(x + width, y + height);
		//vertexData[0].position.x = x + width;
		//vertexData[0].position.y = y + height;
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		//vertexData[1].position.x = x;
		//vertexData[1].position.y = y + height;
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		//vertexData[2].position.x = x;
		//vertexData[2].position.y = y;
		vertexData[2].setUV(0.0f, 0.0f);

		vertexData[3].setPosition(x, y);
		//vertexData[3].position.x = x;
		//vertexData[3].position.y = y;
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		//vertexData[4].position.x = x+width;
		//vertexData[4].position.y = y;
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		//vertexData[5].position.x = x + width;
		//vertexData[5].position.y = y + height;
		vertexData[5].setUV(1.0f, 1.0f);

		for (int i = 0; i < 6; ++i) {
			vertexData[i].setColor(255, 0, 255, 255);
			//vertexData[i].color.r = 255;
			//vertexData[i].color.g = 0;
			//vertexData[i].color.b = 255;
			//vertexData[i].color.a = 255;
		}

		vertexData[1].setColor(0, 0, 255, 255);
		//vertexData[1].color.r = 0;
		//vertexData[1].color.g = 0;
		//vertexData[1].color.b = 255;
		//vertexData[1].color.a = 255;

		vertexData[4].setColor(0, 255, 0, 255);
		//vertexData[4].color.r = 0;
		//vertexData[4].color.g = 255;
		//vertexData[4].color.b = 0;
		//vertexData[4].color.a = 255;

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind buffer
	}

	void Sprite::draw() {
		glBindTexture(GL_TEXTURE_2D, m_texture.id);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glEnableVertexAttribArray(0); //Enable vertex attrib array of index i
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Position attrib pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); //0th index of vertex attribarray, size of two (x and y), of type float. 0 offest because in vertex struct, position is first

		// Color attrib pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); //1st index of vertex attribarray which is color, size of 4 (rgba), of type float.

		// uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)); //2nd index of vertex attribarray

		glDrawArrays(GL_TRIANGLES, 0, 6); //Draw triangles, strat index 0 and 6 vertices

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}