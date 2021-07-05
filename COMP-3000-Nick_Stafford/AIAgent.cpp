#include "AIAgent.h"
namespace Mer
{
	AIAgent::AIAgent()
	{

	}

	void AIAgent::Initialise(Nation* nation)
	{
		this->nation = nation;

		goldPerTurn = this->nation->nationCells.size() / 10.0f;

		armies.Initialise(nation->colour[0], nation->colour[1], nation->colour[2],this->nation->nationCells.size(), nation->capital->centre.x + 1, nation->capital->centre.y + 1);
	}
	void AIAgent::Update()
	{
		if (!nation->atWar.empty() && !isAtWar)
		{
			isAtWar = true;
			toggleSoldiers = true;
		}
		if (isAtWar && nation->atWar.empty())
		{
			isAtWar = false;
			toggleSoldiers = true;
		}

		if (toggleSoldiers)
		{
			soldiersRaised = !soldiersRaised;
			armies.Move(nation->capital->centre.x + 1, nation->capital->centre.y + 1, nation->capital->id);
			toggleSoldiers = false;
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
}