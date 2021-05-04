#pragma once
#include "LoadShaders.h"
#include <iostream>
#include "Nation.h"
#include <sstream>
#include <iomanip>
#include "glm/glm.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr


namespace Mer
{
	class PlayerController
	{
	public:
		PlayerController();
		void Initialise(Nation* selectedNation);
		void Draw(GLuint texture);

		void Tick(float dt);

		int getNationID();
		std::string getNationName();

		std::string getGold();
		std::string getGoldPerTurn();

		void SetTickSpeed(int speed);

		void RaiseSoldiers();

		

	private:

		void UpdateMVP();

		void FindCentreofCapital();

		Nation* nation;
		float gold = 0.0f;
		float prestige = 0.0f;
		int soldiers = 0;

		float soldierXPos = 0.0f, soldierYPos = 0.0f; 
		float soldierXOffset = 0.0f, soldierYOffset = 0.0f;
		float capitalXPos = 0.0f, capitalYPos = 0.0f;
		float xCentering = 0.0f, yCentering = 0.0f;

		bool toggleSoldiers = false;
		bool soldiersRaised = false;

		float goldPerTurn = 0.0f;
		float prestigePerTurn = 0.0f;

		enum Buffer_Counts { Vertex, Texture, NumBuffers = 1 };
		GLuint playerBuffers[NumBuffers];

		GLuint textureBuffer[NumBuffers];

		//glm::vec3 soldiersPos;
		//glm::vec3 capitalPos;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 mvp;

		float gameTickTimer = 10.0f;
		float gameTickAcc = 0.0f;

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
			{-0.947916687,-1.0f,0.0f},//bottom right
			{-0.947916687,-0.907407403,0.0f},//top right
			{-1.0f,-0.907407403,0.0f}//top left
		};

		GLuint playerShader;
	};
}


