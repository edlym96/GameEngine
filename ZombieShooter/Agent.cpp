#include "Agent.h"

// TODO: SORT TILE BY DISTANCE BEFORE HANDLING COLLISION

Agent::Agent()
{
}


Agent::~Agent()
{
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePosition;

	//check the four corners
	//first corner
	m_checkTilePosition(levelData, collideTilePosition, m_position.x, m_position.y);
	
	//second corner
	m_checkTilePosition(levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y);

	//third corner
	m_checkTilePosition(levelData, collideTilePosition, m_position.x, m_position.y + AGENT_WIDTH);

	//fourth corner
	m_checkTilePosition(levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	if (collideTilePosition.size() == 0) {
		return false;
	}

	m_sortTiles(collideTilePosition);

	for (int i = 0; i < collideTilePosition.size(); ++i) {
		m_collideWithTile(collideTilePosition[i]);
	}

	return true;
}

bool Agent::collideWithAgent(Agent* agent){

	const static float MIN_DISTANCE = AGENT_RADIUS+ AGENT_RADIUS;

	glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {
		glm::vec2 collisionDepthVec = glm::normalize(distVec)*collisionDepth;
		m_position += collisionDepthVec;
		agent->m_position -= collisionDepthVec;
		return true;
	}
	return false;
}

void Agent::draw(DawnEngine::SpriteBatch& _spriteBatch) {
	static int textureID = DawnEngine::ResourceManager::getTexture("Textures/circle.png").id;
	
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	
	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;
	_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, m_color);
}

bool Agent::applyDamage(float damage) {
	m_health -= damage;
	if (m_health <= 0) {
		return true;
	}
	return false;
}

glm::vec2 Agent::getPosition() const {
	return m_position;
}

void Agent::m_checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

	// If outside the world just return
	if(cornerPos.x<0 || cornerPos.x >= levelData[0].size() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size()){
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePosition.push_back(cornerPos*(float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH/2.0f));
	}
}

// AABB collision
void Agent::m_collideWithTile(glm::vec2 tilePos) {

	const static float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const static float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	// if we are colliding
	if (xDepth > 0 || yDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
			if (distVec.x < 0) {
				m_position.x -= xDepth;
			}
			else {
				m_position.x += xDepth;
			}
		}
		else {
			if (distVec.y < 0) {
				m_position.y -= yDepth;
			}
			else {
				m_position.y += yDepth;
			}
		}
	}

}

void Agent::m_sortTiles(std::vector<glm::vec2>& collideTilePosition) {
	std::stable_sort(collideTilePosition.begin(), collideTilePosition.end(), [&](glm::vec2 a, glm::vec2 b) {
		glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
		float distance_a = glm::distance(centerPlayerPos, a);
		float distance_b = glm::distance(centerPlayerPos, b);
		return (distance_a < distance_b);
	});
}

/*bool Agent::m_compareTileDistance(glm::vec2 a, glm::vec2 b) {
	glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
	float distance_a = glm::distance(centerPlayerPos, a);
	float distance_b = glm::distance(centerPlayerPos, b);
	return (distance_a>distance_b);

}*/