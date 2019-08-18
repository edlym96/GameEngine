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
	file >> tmp >> m_numHumans; 

	std::getline(file, tmp); // Throw away the rest of the first line

	// Read the level data
	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
		static int i = 0;
	}

	m_spriteBatch.init();
	m_spriteBatch.begin();

	// Create the UVrect
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	DawnEngine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	for (int y = 0; y < m_levelData.size(); ++y) {
		for (int x = 0; x < m_levelData[y].size(); ++x) {
			// get the tile
			char tile = m_levelData[y][x];

			//Get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) {
			case 'B':
				m_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'R':
				m_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'G':
				m_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
				break;
			case 'L':
				m_spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, whiteColor);
				break;
			case '@':
				m_levelData[y][x] = '.';
				m_startPlayerPosition.x = x * TILE_WIDTH;
				m_startPlayerPosition.y = y * TILE_WIDTH;
				break;
			case 'Z':
				m_levelData[y][x] = '.';
				m_zombieStartPositions.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol at %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}
	m_spriteBatch.end();
}

void Level::draw() {
	m_spriteBatch.renderBatch();
}

// Level data is a vector of strings. Length of element is width
int Level::getWidth() const {
	return m_levelData[0].size();
}

// Level data is a vector of strings. Length of vector is height
int Level::getHeight() const {
	return m_levelData.size();
}

int Level::getNumHumans() const {
	return m_numHumans;
}

glm::vec2 Level::getStartPlayerPos() const{
	return m_startPlayerPosition;
}
const std::vector<glm::vec2>& Level::getZombieStartPos() const{
	return m_zombieStartPositions;
}

const std::vector<std::string>& Level::getLevelData() const {
	return m_levelData;
}

Level::~Level()
{
}
