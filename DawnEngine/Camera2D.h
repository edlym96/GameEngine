#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace DawnEngine{
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);

		void update();

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

		bool isObjectInView(const glm::vec2& position, const glm::vec2& dimensions);

		void setPosition(const glm::vec2& newPosition);
		void setScale(float newScale);

		glm::vec2 getPosition();
		float getScale();
		glm::mat4 getCameraMatrix();

	private:
		int m_screenWidth;
		int m_screenHeight;
		bool m_needsMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix;
		glm::mat4 m_orthoMatrix;
	};
}