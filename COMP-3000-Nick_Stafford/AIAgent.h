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

		float gold = 0.0f;
		float prestige = 0.0f;
		int soldiersTotal = 0;

		float goldPerTurn = 0.0f;

		Nation* nation;

		bool isAtWar = false;
		bool soldiersRaised = false;
		bool toggleSoldiers = false;

		int soldierCellID;

		enum Buffer_Counts { Vertex, Texture, NumBuffers = 1 };
		GLuint playerBuffers[NumBuffers];

		GLuint textureBuffer[NumBuffers];

		GLuint agentShader;

		float color[4] = { 1.0f,1.0f,1.0f,1.0 };

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 mvp;

		float soldierXPos = 0.0f, soldierYPos = 0.0f;
		float soldierXOffset = 0.0f, soldierYOffset = 0.0f;
		float xCentering = 0.0f, yCentering = 0.0f;

		GLfloat texVertices[4][2] =
		{
			{0.0f,0.0f},//bottom left
			{1.0f,0.0f},//bottom right
			{1.0f,1.0f},//top right
			{0.0f,1.0f}//top left
		};
		GLfloat Vertices[4][3]
		{
			{-1.0f,-1.0f,0.0f},//bottom left
			{-0.991666675,-1.0f,0.0f},//bottom right
			{-0.991666675,-0.985185206,0.0f},//top right
			{-1.0f,-0.985185206,0.0f}//top left
		};

	};
}


