#pragma once
#include "Human.h"
#include <DawnEngine/InputManager.h>

class Player:public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, DawnEngine::InputManager* inputManager);

	void update(const std::vector<std::string>& levelData, std::vector<Human*> humans, std::vector<Zombie*> zombies);

private:
	DawnEngine::InputManager* _inputManager;
};

