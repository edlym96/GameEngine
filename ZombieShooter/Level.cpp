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

	std::getline(file, tmp); // Throw away the rest of the first line

	// Read the level data
	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
		static int i = 0;
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	// Create the UVrect
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	DawnEngine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	for (int y = 0; y < _levelData.size(); ++y) {
		for (int x = 0; x < _levelData[y].size(); ++x) {
			// get the tile
			char tile = _levelData[y][x];

			//Get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) {
			case 'B':
				_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
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
				_levelData[y][x] = '.';
				_startPlayerPosition.x = x * TILE_WIDTH;
				_startPlayerPosition.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombieStartPositions.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol at %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}
	_spriteBatch.end();
}

void Level::draw() {
	_spriteBatch.renderBatch();
}

// Level data is a vector of strings. Length of element is width
int Level::getWidth() const {
	return _levelData[0].size();
}

// Level data is a vector of strings. Length of vector is height
int Level::getHeight() const {
	return _levelData.size();
}

int Level::getNumHumans() const {
	return _numHumans;
}

glm::vec2 Level::getStartPlayerPos() const{
	return _startPlayerPosition;
}
const std::vector<glm::vec2>& Level::getZombieStartPos() const{
	return _zombieStartPositions;
}

const std::vector<std::string>& Level::getLevelData() const {
	return _levelData;
}

Level::~Level()
{
}
