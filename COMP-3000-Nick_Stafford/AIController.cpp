#include "AIController.h"
namespace Mer
{
	AIController::AIController()
	{

	}
	void AIController::Initialise(std::vector<Nation>* nations, int playerID, std::vector<Cell>* cells)
	{
		for (int i = 0; i < nations->size(); i++)
		{
			if (nations->at(i).id != playerID)
			{
				AIAgent temp;
				temp.Initialise(&nations->at(i), cells);
				agents.push_back(temp);
			}
		}
	}
	void AIController::Update(int gamespeed)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			agents[i].Update(gamespeed);
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

	std::vector<War>* AIController::getWarsOfNation(int nationID)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			if (agents[i].getNationID() == nationID)
			{
				return agents[i].getWars();
			}
		}

		return nullptr;
	}
	void AIController::MakePeaceWith(int AiID, int peaceID)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			if (agents[i].getNationID() == AiID)
			{
				agents[i].MakePeaceWith(peaceID);
			}
		}
	}
	War* AIController::getWarOfWith(int AIid, int withID)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			if (agents[i].getNationID() == AIid)
			{
				return agents[i].getWarWith(withID);
			}
		}
		return nullptr;
	}
}