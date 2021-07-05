#pragma once

#include "Nation.h"
#include "LoadShaders.h"
#include "glm/glm.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#include <GLFW/glfw3.h>
#include "Army.h"

namespace Mer
{
	class AIAgent
	{
	public:
		AIAgent();

		void Initialise(Nation*);

		
		void Update();
		void Tick();
		void Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture);
	private:

		void UpdateMVP(float zoomLevel, float xOffset, float yOffset);

		Army armies;


		float gold = 0.0f;
		float prestige = 0.0f;

		float goldPerTurn = 0.0f;

		Nation* nation;

		bool isAtWar = false;
		bool soldiersRaised = false;
		bool toggleSoldiers = false;

	};
}


