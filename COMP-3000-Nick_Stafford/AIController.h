#pragma once
#include "AIAgent.h"
namespace Mer
{
	class AIController
	{
	public:
		AIController();
		void Initialise(std::vector<Nation>* nations, int playerID);
		void Update();
		void Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture);
		void Tick();
	private:
		std::vector<AIAgent> agents;


	};
}

