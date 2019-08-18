#pragma once
#include <glm/glm.hpp>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/ResourceManager.h>
#include "Level.h"

const float AGENT_WIDTH = 30.0f; // 30.0f
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

// Forward declaration instead of header file to prevent circular dependency
class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies, float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void draw(DawnEngine::SpriteBatch& _spriteBatch);

	// Return true if died
	bool applyDamage(float damage);

	glm::vec2 getPosition() const;

protected:
	void m_checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);

	void m_collideWithTile(glm::vec2 tilePos);
	glm::vec2 m_position;
	DawnEngine::ColorRGBA8 m_color;
	float m_speed;
	float m_health;

};

