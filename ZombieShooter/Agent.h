#pragma once
#include <glm/glm.hpp>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/ResourceManager.h>
#include "Level.h"

const float AGENT_WIDTH = 30;

// Forward declaration instead of header file to prevent circular dependency
class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies) = 0;

	void collideWithLevel(const std::vector<std::string>& levelData);

	void draw(DawnEngine::SpriteBatch& _spriteBatch);

	glm::vec2 getPosition() const;

protected:
	void _checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);

	void _collideWithTile(glm::vec2 tilePos);
	glm::vec2 _position;
	DawnEngine::Color _color;
	float _speed;

};

