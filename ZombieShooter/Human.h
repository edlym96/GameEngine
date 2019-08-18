#pragma once
#include "Agent.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

class Human:public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies, float deltaTime) override;

private:
	glm::vec2 m_direction;
	int m_frames;
};

