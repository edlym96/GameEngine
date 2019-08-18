#include "SpriteBatch.h"

namespace DawnEngine {

	SpriteBatch::SpriteBatch():_vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init() {
		_createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType/* = GlyphSortType::TEXTURE*/) {
		_sortType = sortType;
		_renderBatches.clear();
		// deallocate the glyph pointers
		_glyphs.clear();
	}

	void SpriteBatch::end() {
		// set up all pointers for faster sorting
		_glyphPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphs.size(); ++i) {
			_glyphPointers[i] = &_glyphs[i];
		}
		sortGlyphs();
		_createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {

		_glyphs.emplace_back(destRect,uvRect,texture, depth, color);
	}

	void SpriteBatch::renderBatch() {

		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); ++i) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);

		}

		glBindVertexArray(0);
	}

	void SpriteBatch::_createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(_glyphPointers.size() * 6);

		if (_glyphPointers.empty()) {
			return;
		}
		int offset = 0;
		int currentVertex = 0;
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture); // create copy of render batch using empalce back
		vertices[currentVertex++] = _glyphPointers[0]->topLeft;
		vertices[currentVertex++] = _glyphPointers[0]->bottomLeft;
		vertices[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphPointers[0]->topRight;
		vertices[currentVertex++] = _glyphPointers[0]->topLeft;

		offset += 6;

		for (int cg = 1; cg < _glyphPointers.size(); cg++) {
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}else{
				_renderBatches.back().numVertices += 6;
			}
			vertices[currentVertex++] = _glyphPointers[cg]->topLeft;
			vertices[currentVertex++] = _glyphPointers[cg]->bottomLeft;
			vertices[currentVertex++] = _glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = _glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = _glyphPointers[cg]->topRight;
			vertices[currentVertex++] = _glyphPointers[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//orphan the buffer (instead of calling an existing buffer, call an empty buffer)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::_createVertexArray() {
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}

		glBindVertexArray(_vao);

		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0); //Enable vertex attrib array of index i
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Position attrib pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); //0th index of vertex attribarray, size of two (x and y), of type float. 0 offest because in vertex struct, position is first

		// Color attrib pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); //1st index of vertex attribarray which is color, size of 4 (rgba), of type float.

		// uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)); //2nd index of vertex attribarray

		glBindVertexArray(0); //unbind vertex array, vbo, etc. Nect time vao is bound, everything will already be set up
	}

	void SpriteBatch::sortGlyphs() {
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), _compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), _compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), _compareTexture);
				break;
		}
	}

	bool SpriteBatch::_compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	bool SpriteBatch::_compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}
	bool SpriteBatch::_compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}
}