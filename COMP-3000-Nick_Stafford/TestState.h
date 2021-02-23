#pragma once
#include "State.h"
#include "Game.h"
#include "GameMapController.h"
#include "GUIManager.h"

namespace Mer
{
	class TestState : public State
	{
	public:
		TestState(GameDataRef data);
		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
		void CleanUp();

	private:
		GameDataRef _data;//global data used by program

		enum VAO_IDs { Cells, NumVAOs = 2 };
		GLuint VAOs[NumVAOs];

		bool mousePressed = false;
		

		static bool KeysPressed[348];

		//MAP VARIABLES
		GameMapController GMC;
		GUIManager GUI;

		//CELL SELECTION VARIABLES
		double xpos = 0, ypos = 0;
		int windowH = 0, windowW = 0;

		

		//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


	
	};
}


