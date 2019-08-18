#include "Gun.h"



Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed) :
	_name(name),
	_fireRate(fireRate),
	_bulletsPerShot(bulletsPerShot),
	_spread(spread),
	_bulletDamage(bulletDamage),
	_bulletSpeed(bulletSpeed),
	_frameCounter(0.0f)
{
}


Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime) {
	_frameCounter+=1.0f*deltaTime;
	if (_frameCounter >= _fireRate && isMouseDown) {
		_fire(position, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::_fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-_spread * 3.14159265359f / 180,_spread* 3.14159265359f / 180);

	for (int i = 0; i < _bulletsPerShot; ++i) {
		bullets.emplace_back(position, glm::rotate(direction, randRotate(randomEngine)), _bulletDamage, _bulletSpeed);
	}
}