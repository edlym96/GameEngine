#pragma once
#include <glm/glm.hpp>
#include <DawnEngine/SpriteBatch.h>
#include <DawnEngine/ResourceManager.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(DawnEngine::SpriteBatch& spriteBatch);

	// Returns true when bullets lfietime reaches 0
	bool update();

private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

