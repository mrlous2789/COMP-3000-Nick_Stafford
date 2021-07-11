#pragma once
#include "AIAgent.h"

namespace Mer
{
	class AIController
	{
	public:
		AIController();
		void Initialise(std::vector<Nation>* nations, int playerID, std::vector<Cell>* cells);
		void Update(int gamespeed);
		void Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture);
		void Tick();

		int IsCellOccupied(int id);

		

		Army* getAgentArmy(int nationID);
		std::vector<War>* getWarsOfNation(int nationID);

		void MakePeaceWith(int AiID, int peaceID);
		War* getWarOfWith(int AIid, int withID);

		void RemoveLandFrom(int nationID, std::vector<Cell*> conqueredLand);

		int getAgentsSize();
		AIAgent* getAgent(int id);
	private:
		std::vector<AIAgent> agents;

		

	};
}

