#include "TextureCache.h"
namespace DawnEngine {
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath) {
		// lookup texture to see if it is in map
		auto mit = _textureMap.find(texturePath);

		//check if in the map
		if (mit == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			//Create key-value pair
			//std::pair<std::string, GLTexture>newPair(texturePath, newTexture);

			_textureMap.insert(make_pair(texturePath, newTexture));

			//std::cout << "Loaded Texture!" << std::endl;
			return newTexture;
		}

		//std::cout << "Used Cached Texture!" << std::endl;

		return mit->second;
	}
}