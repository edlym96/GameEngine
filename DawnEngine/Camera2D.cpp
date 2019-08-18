#include "Camera2D.h"


namespace DawnEngine {
	Camera2D::Camera2D() :m_screenWidth(500),
		m_screenHeight(500),
		m_needsMatrixUpdate(true),
		m_scale(1.0f),
		m_position(0.0f, 0.0f),
		m_cameraMatrix(1.0f),
		m_orthoMatrix(1.0f)
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight) {
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
	}

	void Camera2D::update() {
		if (m_needsMatrixUpdate) {

			//Camera Translation
			glm::vec3 translate(-m_position.x+m_screenWidth/2, -m_position.y + m_screenHeight/2, 0.0f);
			m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

			//Camera Scale
			glm::vec3 scale(m_scale, m_scale, 0.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f),scale)*m_cameraMatrix;
			m_needsMatrixUpdate = false;
		}
	}

	bool Camera2D::isObjectInView(const glm::vec2& position, const glm::vec2& dimensions) {
		
		glm::vec2 scaledScreenDimensions = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;

		const static float MIN_DISTANCE_X = dimensions.x/2.0f + scaledScreenDimensions.x/2.0f;
		const static float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		// get center position of object
		glm::vec2 centerPos = position + dimensions / 2.0f;
		// get center position of camera(which is just the position)
		glm::vec2 centerCameraPos = m_position;
		// get distance vector of the camera from the object
		glm::vec2 distVec = centerPos - centerCameraPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		// if we are colliding
		if (xDepth > 0 || yDepth > 0) {
			return true;
		}
		return false;
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		// invert y direction
		screenCoords.y = m_screenHeight - screenCoords.y;

		// make it so 0 is center
		screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);

		// scale coordinates based on zoom
		screenCoords /= m_scale;

		//translate with the camera position
		screenCoords += m_position;

		return screenCoords;
	}

	void Camera2D::setPosition(const glm::vec2& newPosition) {
		m_position = newPosition;
		m_needsMatrixUpdate = true;
	}

	void Camera2D::setScale(float newScale) { 
		m_scale = newScale; 
		m_needsMatrixUpdate = true;
	}

	glm::vec2 Camera2D::getPosition() { return m_position; }

	float Camera2D::getScale() { return m_scale; }

	glm::mat4 Camera2D::getCameraMatrix() { return m_cameraMatrix; }
}