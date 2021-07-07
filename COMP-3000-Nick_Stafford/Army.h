#pragma once
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
	class Army
	{

	public:
		Army();
		void Initialise(float red, float green, float blue, int maxSoldiers, float soldierXpos, float soldierYpos, int nationID);
		void Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture);
		void Move(float xPos, float yPos, int id);

		void Selected();

		int nationID;

		int maxSoldiers;

		int totalSoldiers;
		float morale = 100.0f;
		
		float attack;
		float defence;
		
		int locationID;

		float soldierXPos = 0.0f, soldierYPos = 0.0f;
		float soldierXOffset = 0.0f, soldierYOffset = 0.0f;
		float xCentering = 0.0f, yCentering = 0.0f;

		bool enganged = false;

		bool broken = false;
	private:
		void UpdateMVP(float zoomLevel, float xOffset, float yOffset);

		

		enum Buffer_Counts { Vertex, Texture, NumBuffers = 1 };
		GLuint playerBuffers[NumBuffers];

		GLuint textureBuffer[NumBuffers];

		GLuint soldierShader;

		float color[4] = { 1.0f,1.0f,1.0f,0.8f };

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 mvp;



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


