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
	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);


private:
	std::string m_name;
	int m_fireRate; // fireRate in terms of frames
	int m_bulletsPerShot;
	float m_spread; //accuracy
	float m_bulletSpeed;
	float m_bulletDamage;
	float m_frameCounter;

	void m_fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
};

