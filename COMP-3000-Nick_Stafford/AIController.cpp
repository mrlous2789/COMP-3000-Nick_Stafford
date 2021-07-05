#include "AIController.h"
namespace Mer
{
	AIController::AIController()
	{

	}
	void AIController::Initialise(std::vector<Nation>* nations, int playerID)
	{
		for (int i = 0; i < nations->size(); i++)
		{
			if (nations->at(i).id != playerID)
			{
				AIAgent temp;
				temp.Initialise(&nations->at(i));
				agents.push_back(temp);
			}
		}
	}
	void AIController::Update()
	{
		for (int i = 0; i < agents.size(); i++)
		{
			agents[i].Update();
		}
	}
	void AIController::Tick()
	{
		for (int i = 0; i < agents.size(); i++)
		{
			agents[i].Tick();
		}
	}
	void AIController::Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			agents[i].Draw(zoomLevel, xOffset, yOffset, texture);
		}
	}
	int AIController::IsCellOccupied(int id)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			if (agents[i].getArmyLocation() == id)
			{
				return agents[i].getNationID();
			}
		}
		return -1;
	}

	Army* AIController::getAgentArmy(int nationID)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			if (agents[i].getNationID() == nationID)
			{
				return agents[i].getArmy();
			}
		}

		return nullptr;
	}
}