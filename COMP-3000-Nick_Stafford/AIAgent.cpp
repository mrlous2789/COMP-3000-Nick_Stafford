#include "AIAgent.h"
namespace Mer
{
	AIAgent::AIAgent()
	{

	}

	void AIAgent::Initialise(Nation* nation, std::vector<Cell>* cells)
	{
		this->nation = nation;
		this->cells = cells;

		goldPerTurn = this->nation->nationCells.size() / 10.0f;

		armies.Initialise(nation->colour[0], nation->colour[1], nation->colour[2],this->nation->nationCells.size(), nation->capital->centre.x + 1, nation->capital->centre.y + 1, nation->id);
		PF.Initialise(this->cells);
	}
	void AIAgent::Update(int gameSpeed)
	{
		if (!wars.empty() && !isAtWar)
		{
			isAtWar = true;
			toggleSoldiers = true;
		}
		if (isAtWar && wars.empty())
		{
			isAtWar = false;
			toggleSoldiers = true;
		}

		if (armies.broken && !soldiersMoving)
		{
			std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
			std::uniform_int_distribution<int> distribution(0, nation->nationCells.size());

			route = PF.CalculatePath(&cells->at(armies.locationID), nation->nationCells[distribution(generator)]);
			routePos = 0;
			soldiersMoving = true;
		}
		if (soldiersMoving && armies.enganged)
		{
			soldiersMoving = false;
		}

		if (toggleSoldiers)
		{
			soldiersRaised = !soldiersRaised;
			armies.Move(nation->capital->centre.x + 1, nation->capital->centre.y + 1, nation->capital->id);
			cells->at(nation->capital->id).occupier = &armies;
			toggleSoldiers = false;
		}

		if (soldiersMoving)
		{
			smProgress += soldierMoveSpeed * gameSpeed;

			if (smProgress >= 0.2f)
			{
				routePos++;
				smProgress = 0.0f;


				if (route[routePos]->occupier == nullptr || armies.broken)
				{
					cells->at(armies.locationID).occupier = nullptr;
					armies.Move(route[routePos]->centre.x + 1, route[routePos]->centre.y + 1, route[routePos]->id);
					route[routePos]->occupier = &armies;
				}
				else
				{
					if (AlreadyAtWar(route[routePos]->occupier->nationID))
					{
						if (!route[routePos]->occupier->broken)
						{
							BC.NewBattle(&armies, route[routePos]->occupier, route[routePos]->biome);
						}
						routePos = route.size() - 1;
					}
					else
					{
						routePos = route.size() - 1;
					}
				}

			}
			if (routePos == route.size() - 1)
			{
				soldiersMoving = false;
				if (armies.broken)
				{
					armies.broken = false;
				}
			}
		}
	}
	void AIAgent::Tick()
	{

	}
	void AIAgent::Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture)
	{
		if (soldiersRaised)
		{
			armies.Draw(zoomLevel, xOffset, yOffset, texture);
		}
	}
	int AIAgent::getArmyLocation()
	{
		return armies.locationID;
	}
	int AIAgent::getNationID()
	{
		return nation->id;
	}
	Army* AIAgent::getArmy()
	{
		return &armies;
	}
	std::vector<War>* AIAgent::getWars()
	{
		return &wars;
	}
	void AIAgent::MakePeaceWith(int id)
	{
		for (int i = 0; i < wars.size(); i++)
		{
			if (wars[i].nation->id == id)
			{
				wars.erase(wars.begin() + i);
			}
		}
	}
	bool AIAgent::AlreadyAtWar(int id)
	{
		for (int i = 0; i < wars.size(); i++)
		{
			if (wars[i].nation->id == id)
			{
				return true;
			}
		}
		return false;
	}

	War* AIAgent::getWarWith(int id)
	{
		for (int i = 0; i < wars.size(); i++)
		{
			if (wars[i].nation->id == id)
			{
				return &wars[i];
			}
		}
		return nullptr;
	}
	void AIAgent::RemoveLand(std::vector<Cell*> conqueredLand)
	{
		for (int i = 0; i < conqueredLand.size(); i++)
		{
			for (int j = 0; j < nation->nationCells.size(); j++)
			{
				if (nation->nationCells[j] == conqueredLand[i])
				{
					nation->nationCells.erase(nation->nationCells.begin() + j);
				}
			}
		}
	}
	void AIAgent::UpdateMaxSoldiers()
	{
		armies.updateMaxSoldiers(nation->nationCells.size());
	}
}