#include "Level.h"



Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (file.fail()) {
		DawnEngine::fatalError("Failed to open " + fileName);
	}

	// Throw away the first string in tmp
	std::string tmp;
	file >> tmp >> _numHumans; 

	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
		static int i = 0;
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	// Create the UVrect
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	DawnEngine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	for (int i = 0; i < _levelData.size(); ++i) {
		for (int j = 0; j < _levelData[i].size(); ++j) {
			// get the tile
			char tile = _levelData[i][j];

			//Get dest rect
			glm::vec4 destRect(j * TILE_WIDTH, i * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) {
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, whiteColor);
				break;
			case '@':
				_startPlayerPosition.x = j * TILE_WIDTH;
				_startPlayerPosition.y = i * TILE_WIDTH;
				break;
			case 'Z':
				_zombieStartPositions.emplace_back(j*TILE_WIDTH, i*TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol at %c at (%d,%d)", tile, j, i);
				break;
			}
		}
	}
	_spriteBatch.end();
}

void Level::draw() {
	_spriteBatch.renderBatch();
}

Level::~Level()
{
}
