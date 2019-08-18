#include "Zombie.h"



Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;
	_health = 150;
	_color = DawnEngine::ColorRGBA8(0, 160, 0, 255);
	/*_color.r = 0;
	_color.g = 160;
	_color.b = 0;
	_color.a = 255;*/
}

void Zombie::update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies, float deltaTime) {

	Human* closestHuman = _getNearestHuman(humans);

	if (closestHuman) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction * _speed*deltaTime;
	}

	collideWithLevel(levelData);
}

Human* Zombie::_getNearestHuman(std::vector<Human*> humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = 99999999.0f;

	for (int i = 0; i < humans.size(); ++i) {
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}