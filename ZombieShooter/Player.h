#pragma once
#include "Human.h"
#include "Gun.h"
#include "Bullet.h"
#include <DawnEngine/InputManager.h>
#include <DawnEngine/Camera2D.h>

class Player:public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, DawnEngine::InputManager* inputManager, DawnEngine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies, float deltaTime) override;

private:
	DawnEngine::InputManager* mm_inputManager;
	std::vector<Gun*> m_guns;
	int m_currentGunIndex;
	DawnEngine::Camera2D* m_camera;
	std::vector<Bullet>* m_bullets;
};

