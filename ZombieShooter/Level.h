#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <DawnEngine/Errors.h>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/ResourceManager.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	// Load the level
	Level(const std::string& fileName);
	~Level();

	void draw();

private:
	std::vector<std::string> _levelData;
	int _numHumans;
	DawnEngine::SpriteBatch _spriteBatch;

	DawnEngine::ResourceManager* _resourceManager; //Pointer to the manager in maingame

	glm::ivec2 _startPlayerPosition;
	std::vector<glm::ivec2> _zombieStartPositions;
};

