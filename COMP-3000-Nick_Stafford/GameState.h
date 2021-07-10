#pragma once
#include "State.h"
#include "Game.h"
#include "GUIManager.h"
#include "PlayerController.h"


namespace Mer
{
	class GameState : public State
	{
	public:
		GameState(GameDataRef data);
		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
		void CleanUp();
	private:
		GameDataRef _data;//global data used by program

		enum VAO_IDs { Cells, NumVAOs = 2 };
		GLuint VAOs[NumVAOs];

		bool leftMousePressed = false;
		bool rightMousePressed = false;
		
		bool showDiplomacyPanel = false;
		bool showWarsPanel = false;
		bool showConstructionPanel = false;

		int diplomacyOffset = 0;

		//MAP VARIABLES
		GUIManager GUI;
		PlayerController PLC;

		//CELL SELECTION VARIABLES
		double xpos = 0, ypos = 0;
		int windowH = 0, windowW = 0;



	};
}

