#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/ResourceManager.h>
#include "Agent.h"

class Human;
class Zombie;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	// WHen update returns true, delete bullet
	bool update(const std::vector<std::string>& levelData, float deltaTime);
	void draw(DawnEngine::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* agent);
	float getDamage() const ;

private:
	bool m_collideWithLevel(const std::vector<std::string>& levelData);
	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_damage;
	float m_speed;
};

