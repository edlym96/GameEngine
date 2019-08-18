#pragma once
#include <map>
#include "GLTexture.h"
#include "ImageLoader.h"
#include <iostream>
namespace DawnEngine {
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> m_textureMap;
	};

}