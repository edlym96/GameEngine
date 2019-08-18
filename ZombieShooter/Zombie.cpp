#include "Zombie.h"



Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos) {
	m_speed = speed;
	m_position = pos;
	m_health = 150;
	m_color = DawnEngine::ColorRGBA8(0, 160, 0, 255);
	/*_color.r = 0;
	_color.g = 160;
	_color.b = 0;
	_color.a = 255;*/
}

void Zombie::update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies, float deltaTime) {

	Human* closestHuman = m_getNearestHuman(humans);

	if (closestHuman) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += direction * m_speed*deltaTime;
	}

	collideWithLevel(levelData);
}

Human* Zombie::m_getNearestHuman(std::vector<Human*> humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = 99999999.0f;

	for (int i = 0; i < humans.size(); ++i) {
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}