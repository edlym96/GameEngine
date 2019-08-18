#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
namespace DawnEngine{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();
		
		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		void setMouseCoords(float x, float y);

		// return true if key is held down
		bool isKeyDown(unsigned int keyID);

		// return true if key was just pressed
		bool isKeyPressed(unsigned int keyID);


		//getters
		glm::vec2 getMouseCoords() const;
	private:
		std::unordered_map<unsigned int, bool> _previousKeyMap;
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
		bool _wasKeyDown(unsigned int keyID);
	};

}