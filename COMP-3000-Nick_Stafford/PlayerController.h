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
#include "GameMapController.h"
#include<cmath>

#include "Pathfinding.h"

namespace Mer
{
	class PlayerController
	{
	public:
		PlayerController();
		void Initialise(int screenWidth, int screenHeight);
		void Draw(GLuint texture);

		void Tick();

		void Update(float dt);

		int getNationID();
		std::string getNationName();

		void WarWith(int id);
		void PeaceWith(int id);

		std::string getGold();
		std::string getGoldPerTurn();

		void SetTickSpeed(int speed);

		void RaiseSoldiers();

		bool areSoldiersSelected();

#define PI 3.14159265

		std::string getSelectedCellNationName();
		std::string getSelectedCellReligionName();
		std::string getSelectedCellCultureName();

		int getNationsCount();
		std::string getNationsName(int id);

		bool ProcessMouseClick(double mouseX, double mouseY);
		bool ProcessLeftMouseRelease(double mouseX, double mouseY);
		bool ProcessRightMouseRelease(double mouseX, double mouseY);

		void HandleInput();

		int getSoldiersTotal();

		float getSoldierScreenX();
		float getSoldierScreenY();

		void UpdateMapDrawMode(int drawMode);

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	private:

		GameMapController GMC;
		Pathfinding PF;

		bool AlreadyAtWar(int id);

		std::vector<Cell*> route;
		int routePos = 0;
		double smProgress = 0.0f;

		void UpdateMVP(float zoomLevel, float xOffset, float yOffset);


		bool soldiersSelected = false;
		glm::vec2 soldierMoveTo;

		Nation* nation;
		float gold = 0.0f;
		float prestige = 0.0f;
		int soldiers = 0;

		float soldierXPos = 0.0f, soldierYPos = 0.0f; 
		float soldierXOffset = 0.0f, soldierYOffset = 0.0f;
		float xCentering = 0.0f, yCentering = 0.0f;

		int soldierCellID = 0;


		float soldierMoveSpeed = 0.001f;
		float xDifference = 0.0f, yDifference = 0.0f, xyRatio = 0.0f;


		float soldierMoveGradient = 0.0f;
		float soldierYIntercept = 0.0f;

		bool toggleSoldiers = false;
		bool soldiersRaised = false;

		bool soldierMoving = false;

		int soldiersTotal = 0;

		float goldPerTurn = 0.0f;
		float prestigePerTurn = 0.0f;

		float screenWidth = 0.0f, screenHeight = 0.0f;

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
		int gameSpeed = 3;

		float color[4] = {1.0f,1.0f,1.0f,0.8};

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

		GLuint playerShader;

		static bool KeysPressed[348];

		static bool isZoomOut;
		static bool isZoomIn;

	};
}


