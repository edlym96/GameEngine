#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <DawnEngine/Errors.h>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/ResourceManager.h>

const int TILE_WIDTH = 32;

class Level
{
public:
	// Load the level
	Level(const std::string& fileName);
	~Level();

	void draw();

	//getters
	int getWidth() const;
	int getHeight() const;
	int getNumHumans() const;
	glm::vec2 getStartPlayerPos() const;
	const std::vector<glm::vec2>& getZombieStartPos() const;
	const std::vector<std::string>& getLevelData() const;
private:
	std::vector<std::string> m_levelData;
	int m_numHumans;
	DawnEngine::SpriteBatch m_spriteBatch;

	DawnEngine::ResourceManager* m_resourceManager; //Pointer to the manager in maingame

	glm::vec2 m_startPlayerPosition;
	std::vector<glm::vec2> m_zombieStartPositions;
};

