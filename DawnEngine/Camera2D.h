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
		int _screenWidth;
		int _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};
}