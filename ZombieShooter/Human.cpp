#include "Human.h"

Human::Human():_frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos) {

	// Declare a static random number generator that is shared for all humans
	static std::mt19937 randomEngine(time(nullptr)); //seed the randomengine with time
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_health = 20;

	_color.r = 200;
	_color.g = 0;
	_color.b = 200;
	_color.a = 255;

	_speed = speed;
	_position = pos;
	//Get random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	// Handle the very unlikely case direction is initialised to zero
	if (_direction.length() == 0) {
		_direction = glm::vec2(1.0f, 0.0f);
	}

	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies) {

	static std::mt19937 randomEngine(time(nullptr)); //seed the randomengine with time
	static std::uniform_real_distribution<float> randRotate(-40.0f * 3.14159265359f / 180, 40.0f * 3.14159265359f / 180);

	_position += _direction * _speed;

	// Randomly change direction every 20 frames
	if (_frames == 20) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else {
		_frames++;
	}

	if (collideWithLevel(levelData)) {
		_direction = _direction = glm::rotate(_direction, randRotate(randomEngine));
	};
}