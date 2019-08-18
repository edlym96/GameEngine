#include "SpriteBatch.h"

namespace DawnEngine {

	SpriteBatch::SpriteBatch():m_vbo(0), m_vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init() {
		m_createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType/* = GlyphSortType::TEXTURE*/) {
		m_sortType = sortType;
		m_renderBatches.clear();
		// deallocate the glyph pointers
		m_glyphs.clear();
	}

	void SpriteBatch::end() {
		// set up all pointers for faster sorting
		m_glyphPointers.resize(m_glyphs.size());
		for (int i = 0; i < m_glyphs.size(); ++i) {
			m_glyphPointers[i] = &m_glyphs[i];
		}
		sortGlyphs();
		m_createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {

		m_glyphs.emplace_back(destRect,uvRect,texture, depth, color);
	}

	void SpriteBatch::renderBatch() {

		glBindVertexArray(m_vao);

		for (int i = 0; i < m_renderBatches.size(); ++i) {
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);

		}

		glBindVertexArray(0);
	}

	void SpriteBatch::m_createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(m_glyphPointers.size() * 6);

		if (m_glyphPointers.empty()) {
			return;
		}
		int offset = 0;
		int currentVertex = 0;
		m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture); // create copy of render batch using empalce back
		vertices[currentVertex++] = m_glyphPointers[0]->topLeft;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomLeft;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[0]->topRight;
		vertices[currentVertex++] = m_glyphPointers[0]->topLeft;

		offset += 6;

		for (int cg = 1; cg < m_glyphPointers.size(); cg++) {
			if (m_glyphPointers[cg]->texture != m_glyphPointers[cg - 1]->texture) {
				m_renderBatches.emplace_back(offset, 6, m_glyphPointers[cg]->texture);
			}else{
				m_renderBatches.back().numVertices += 6;
			}
			vertices[currentVertex++] = m_glyphPointers[cg]->topLeft;
			vertices[currentVertex++] = m_glyphPointers[cg]->bottomLeft;
			vertices[currentVertex++] = m_glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = m_glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = m_glyphPointers[cg]->topRight;
			vertices[currentVertex++] = m_glyphPointers[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//orphan the buffer (instead of calling an existing buffer, call an empty buffer)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::m_createVertexArray() {
		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
		}

		glBindVertexArray(m_vao);

		if (m_vbo == 0) {
			glGenBuffers(1, &m_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

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
		switch (m_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), m_compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), m_compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), m_compareTexture);
				break;
		}
	}

	bool SpriteBatch::m_compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	bool SpriteBatch::m_compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}
	bool SpriteBatch::m_compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}
}