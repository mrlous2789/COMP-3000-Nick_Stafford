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
#include "BattleController.h"

#include "AIController.h"

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
		bool isArmyEnganged();
		
		Battle* getBattleInfo();

		std::string getAttackerName();
		std::string getDefenderName();

		std::string getAttackerMorale();
		std::string getDefenderMorale();

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

		void UpdateMapDrawMode(int drawMode);

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		int getWarsSize();
		std::string getWarsNationName(int id);
		std::string getWarsWarScore(int id);

		void StartPeaceDeal(int id);

		std::string getPeaceWarScoreWith();
		std::string getCurrentWarScoreCost();

		void AcceptPeaceDeal();
		void CancelPeaceDeal();

		std::string getMakingPeaceWithName();

		bool isMakingPeace();
	private:

		GameMapController GMC;
		Pathfinding PF;
		AIController AIC;
		Army army;
		BattleController BC;

		std::vector<Battle> finishedBattles;

		bool AlreadyAtWar(int id);
		int makingPeaceWith = -1;

		std::vector<Cell*> conqueredLand;

		War* getWarWith(int id);

		std::vector<Cell*> route;
		int routePos = 0;
		double smProgress = 0.0f;

		std::vector<War> wars;
		
		Cell* lastSelectedCell;

		bool makingPeaceDeal = false;

		bool soldiersSelected = false;
		glm::vec2 soldierMoveTo;

		Nation* nation;
		float gold = 0.0f;
		float prestige = 0.0f;

		float soldierMoveSpeed = 0.001f;

		bool toggleSoldiers = false;
		bool soldiersRaised = false;

		bool soldierMoving = false;

		float gameTickAcc = 0.0f, gameTickTimer = 6.5f;
		int gameSpeed = 3;

		float goldPerTurn = 0.0f;
		float prestigePerTurn = 0.0f;

		float screenWidth = 0.0f, screenHeight = 0.0f;


		static bool KeysPressed[348];

		static bool isZoomOut;
		static bool isZoomIn;

	};
}


