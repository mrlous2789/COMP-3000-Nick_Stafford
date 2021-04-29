#pragma once
#include "GameState.h"


namespace Mer
{
	class MainMenuState : public State
	{
	public:
		MainMenuState(GameDataRef data);
		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
		void CleanUp();

	private:
		GameDataRef _data;//global data used by program

		GUIManager GUI;

		enum Buffer_Counts { vertex, texture, NumBuffers = 2};
		GLuint backgroundBuffers[NumBuffers];

		bool mousePressed = false;
		double xpos = 0, ypos = 0;
		int windowH = 0, windowW = 0;

		GLfloat backgroundCoords[4][3]
		{
			{-1.0f,1.0f,0.0f},
			{1.0f,1.0f,0.0f},
			{1.0f,-1.0f,0.0f},
			{-1.0f,-1.0f,0.0f},
		};

		GLfloat texVertices[4][2] =
		{
			{0.0f,1.0f},//top left
			{1.0f,1.0f},//top right
			{1.0f,0.0f},//bottom right
			{0.0f,0.0f}//bottom left	
		};

	}; 
}


