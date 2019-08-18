#include "Gun.h"



Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed) :
	m_name(name),
	m_fireRate(fireRate),
	m_bulletsPerShot(bulletsPerShot),
	m_spread(spread),
	m_bulletDamage(bulletDamage),
	m_bulletSpeed(bulletSpeed),
	m_frameCounter(0.0f)
{
}


Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime) {
	m_frameCounter+=1.0f*deltaTime;
	if (m_frameCounter >= m_fireRate && isMouseDown) {
		m_fire(position, direction, bullets);
		m_frameCounter = 0;
	}
}

void Gun::m_fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-m_spread * 3.14159265359f / 180,m_spread* 3.14159265359f / 180);

	for (int i = 0; i < m_bulletsPerShot; ++i) {
		bullets.emplace_back(position, glm::rotate(direction, randRotate(randomEngine)), m_bulletDamage, m_bulletSpeed);
	}
}