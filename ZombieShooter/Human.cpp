#include "Human.h"

Human::Human():m_frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos) {

	// Declare a static random number generator that is shared for all humans
	static std::mt19937 randomEngine(time(nullptr)); //seed the randomengine with time
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_health = 20;

	m_color.r = 200;
	m_color.g = 0;
	m_color.b = 200;
	m_color.a = 255;

	m_speed = speed;
	m_position = pos;
	//Get random direction
	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	// Handle the very unlikely case direction is initialised to zero
	if (m_direction.length() == 0) {
		m_direction = glm::vec2(1.0f, 0.0f);
	}

	m_direction = glm::normalize(m_direction);
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies, float deltaTime) {

	static std::mt19937 randomEngine(time(nullptr)); //seed the randomengine with time
	static std::uniform_real_distribution<float> randRotate(-40.0f * 3.14159265359f / 180, 40.0f * 3.14159265359f / 180);

	m_position += m_direction * m_speed*deltaTime;

	// Randomly change direction every 20 frames
	if (m_frames == 20) {
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		m_frames = 0;
	}
	else {
		m_frames++;
	}

	if (collideWithLevel(levelData)) {
		m_direction = m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	};
}