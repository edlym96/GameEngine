#include "Player.h"



Player::Player():m_currentGunIndex(-1)
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, DawnEngine::InputManager* inputManager, DawnEngine::Camera2D* camera,std::vector<Bullet>* bullets) {
	m_speed = speed;
	m_position = pos;
	mm_inputManager = inputManager;
	m_color.r = 0;
	m_color.g = 0;
	m_color.b = 185;
	m_color.a = 255;
	m_health = 150;
	m_camera = camera;
	m_bullets = bullets;
}

void Player::addGun(Gun* gun) {
	m_guns.push_back(gun);

	//If no gun equipped, equip gun
	if (m_currentGunIndex == -1) {
		m_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies, float deltaTime) {
	if (mm_inputManager->isKeyDown(SDLK_w)) {
		m_position.y += m_speed*deltaTime;
	}
	else if (mm_inputManager->isKeyDown(SDLK_s)) {
		m_position.y -= m_speed * deltaTime;
	}
	if (mm_inputManager->isKeyDown(SDLK_d)) {
		m_position.x += m_speed * deltaTime;
	}
	else if (mm_inputManager->isKeyDown(SDLK_a)) {
		m_position.x -= m_speed * deltaTime;
	}

	if (mm_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0) {
		m_currentGunIndex = 0;
	}else if (mm_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 1) {
		m_currentGunIndex = 1;
	}else if (mm_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 2) {
		m_currentGunIndex = 2;
	}

	if (m_currentGunIndex != -1) {
		glm::vec2 mouseCoords = mm_inputManager->getMouseCoords();
		mouseCoords = m_camera->convertScreenToWorld(mouseCoords);
		glm::vec2 centerPosition = m_position+AGENT_RADIUS;
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);
		m_guns[m_currentGunIndex]->update(mm_inputManager->isKeyDown(SDL_BUTTON_LEFT), centerPosition-glm::vec2(BULLET_RADIUS), direction, *m_bullets, deltaTime);
	}

	collideWithLevel(levelData);
}