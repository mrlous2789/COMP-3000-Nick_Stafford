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
#include "Wars.h"
#include "Pathfinding.h"
#include <random>
#include "BattleController.h"
#include <chrono>

namespace Mer
{
	class AIAgent
	{
	public:
		AIAgent();

		void Initialise(Nation*, std::vector<Cell>* cells);

		
		void Update(int gameSpeed);
		void Tick();
		void Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture);

		int getArmyLocation();
		void setArmyLocation(int id);
		int getNationID();

		std::vector<War>* getWars();
		War* getWarWith(int id);

		void MakePeaceWith(int id);
		void RemoveLand(std::vector<Cell*> conqueredLand);
		void UpdateMaxSoldiers();

		bool areSoldiersRaised();
		void setSoldiersRaised(int soldiersRaised);

		float getGold();
		void setGold(float gold);
		

		Army* getArmy();
	private:

		Pathfinding PF;
		BattleController BC;

		std::vector<Cell*> route;

		std::vector<Cell>* cells;

		bool AlreadyAtWar(int id);




		Army armies;

		std::vector<War> wars;

		int routePos = 0;
		double smProgress = 0.0f;
		float soldierMoveSpeed = 0.001f;

		float gold = 0.0f;
		float prestige = 0.0f;

		float goldPerTurn = 0.0f;

		Nation* nation;


		bool soldiersMoving = false;
		bool isAtWar = false;
		bool soldiersRaised = false;
		bool toggleSoldiers = false;

	};
}


