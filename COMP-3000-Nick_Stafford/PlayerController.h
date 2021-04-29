#pragma once
#include "LoadShaders.h"
#include <iostream>
#include "Nation.h"
#include <sstream>
#include <iomanip>

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

	private:

		Nation* nation;
		float gold = 0.0f;
		float prestige = 0.0f;
		int soldiers = 0;

		

		float goldPerTurn = 0.0f;
		float prestigePerTurn = 0.0f;

		enum Buffer_Counts { Vertex, Texture, NumBuffers = 2 };
		GLuint playerBuffers[NumBuffers];

		float gameTickTimer = 3.0f;
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
			{1.0f,-1.0f,0.0f},//bottom right
			{1.0f,1.0f,0.0f},//top right
			{-1.0f,1.0f,0.0f}//top left
		};

		GLuint playerShader;
	};
}


