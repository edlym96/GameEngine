#include "Sprite.h"

Sprite::Sprite():_vboID(0)
{

}


Sprite::~Sprite()
{
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(float x, float y, float width, float height) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	//Create buffer it hasnt already been generated
	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6];//6 vertices, x and y for each
	vertexData[0].position.x = x + width;
	vertexData[0].position.y = y + height;

	vertexData[1].position.x = x;
	vertexData[1].position.y = y + height;

	vertexData[2].position.x = x;
	vertexData[2].position.y = y;

	vertexData[3].position.x = x;
	vertexData[3].position.y = y;

	vertexData[4].position.x = x+width;
	vertexData[4].position.y = y;

	vertexData[5].position.x = x + width;
	vertexData[5].position.y = y + height;

	for (int i = 0; i < 6; ++i) {
		vertexData[i].color.r = 255;
		vertexData[i].color.g = 0;
		vertexData[i].color.b = 255;
		vertexData[i].color.a = 255;
	}

	vertexData[1].color.r = 0;
	vertexData[1].color.g = 0;
	vertexData[1].color.b = 255;
	vertexData[1].color.a = 255;

	vertexData[4].color.r = 0;
	vertexData[4].color.g = 255;
	vertexData[4].color.b = 0;
	vertexData[4].color.a = 255;

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind buffer
}

void Sprite::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	//glEnableVertexAttribArray(0); //Enable vertex attrib array of index i (only position, no lighting)

	// Position attrib pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); //0th index of vertex attribarray, size of two (x and y), of type float. 0 offest because in vertex struct, position is first

	// Color attrib pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); //1st index of vertex attribarray which is color, size of two (x and y), of type float.

	glDrawArrays(GL_TRIANGLES, 0, 6); //Draw triangles, strat index 0 and 6 vertices

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}