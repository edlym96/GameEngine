#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>
#include <random>
#include <ctime>
#include "Bullet.h"


class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
	~Gun();
	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);


private:
	std::string _name;
	int _fireRate; // fireRate in terms of frames
	int _bulletsPerShot;
	float _spread; //accuracy
	float _bulletSpeed;
	float _bulletDamage;
	int _frameCounter;

	void _fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
};

