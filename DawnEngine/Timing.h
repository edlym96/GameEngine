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
		void m_calculateFPS();

		float m_fps;
		float m_frameTime;
		float m_maxFPS;
		unsigned int m_startTicks;
	};
}