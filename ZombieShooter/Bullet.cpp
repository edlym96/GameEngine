#include "Bullet.h"



Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed):
	m_position(position),
	m_direction(direction),
	m_damage(damage),
	m_speed(speed)
{
}


Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime) {
	m_position += m_direction * m_speed*deltaTime;
	return m_collideWithLevel(levelData);
}

void Bullet::draw(DawnEngine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect(m_position.x, m_position.y, BULLET_RADIUS*2, BULLET_RADIUS*2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	DawnEngine::ColorRGBA8 color;
	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;
	spriteBatch.draw(destRect, uvRect, DawnEngine::ResourceManager::getTexture("Textures/circle.png").id,0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent) {
	const static float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = m_position + glm::vec2(BULLET_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {
		return true;
	}
	return false;
}

float Bullet::getDamage() const {
	return m_damage;
}

bool Bullet::m_collideWithLevel(const std::vector<std::string>& levelData) {
	glm::ivec2 gridPosition;
	gridPosition.x = floor(m_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(m_position.y / (float)TILE_WIDTH);

	// If outside the world just return true
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
		return true;
	}

	return (levelData[gridPosition.y][gridPosition.x] != '.');
}