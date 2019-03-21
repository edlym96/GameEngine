#pragma once
#include <SDL/SDL.h>

namespace DawnEngine {
	class FpsLimiter {
	public:
		FpsLimiter();
		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void beginFrame();

		//end will return current fps
		float endFrame();
	
	private:
		void _calculateFPS();

		float _fps;
		float _frameTime;
		float _maxFPS;
		unsigned int _startTicks;
	};
}