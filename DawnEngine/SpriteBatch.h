#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.h"
#include <algorithm>

namespace DawnEngine {
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :offset(Offset),
			numVertices(NumVertices),
			texture(Texture) {}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};
	struct Glyph {
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init(); // initialise sprite batches

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE); //sets up everything for drawing (how glyphs should be sorted, etc.)
		void end(); // post processing after drawing is complete

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);

		void renderBatch();

	private:
		void _createRenderBatches();
		void _createVertexArray();
		void sortGlyphs();

		static bool _compareFrontToBack(Glyph* a, Glyph* b);
		static bool _compareBackToFront(Glyph* a, Glyph* b);
		static bool _compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;

		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphs; //use pointers because sorting pointers is more memory efficient than sorting the structs themselves
		std::vector<RenderBatch> _renderBatches;
	};
}