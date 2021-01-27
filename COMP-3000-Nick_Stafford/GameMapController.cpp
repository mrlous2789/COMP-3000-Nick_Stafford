#include "GameMapController.h"
namespace Mer
{
	GameMapController::GameMapController()
	{

	}
	bool GameMapController::LoadFromFile(std::string location, std::string mapname)
	{
		//construct file locations and load them
		cells = reader.ReadCellFile(location + mapname + ".geojson");
		rivers = reader.ReadRiversFile(location  + mapname + "_rivers.geojson");
		nations = reader.ReadNationFile(location  + mapname + "_nations.mer");
		cultures = reader.ReadCutlureFile(location + mapname + "_cultures.mer");
		religions = reader.ReadReligionFile(location + mapname + "_religions.mer");

		if (cells.empty())
			return false;
		else
			return true;
	}

	Nation GameMapController::getNationById(int id)
	{
		Nation nat;
		nat.id = -1;
		nat.capitalId = -1;

		if (id >= 0 && id < nations.size())//if nation at index of id is correct nation return it
			if (nations[id].id == id)
				return nations[id];

		for (int i = 0; i < nations.size(); i++)//loop through all nations to find correct one
			if (nations[i].id == id)
				return nations[i];

		return nat;
	}
	Culture GameMapController::getCultureById(int id)
	{
		Culture cult;
		cult.id = -1;
		if (id >= 0 && id < cultures.size())//if culture at index of id is correct culture return it
			if (cultures[id].id == id)
				return cultures[id];

		for (int i = 0; i < cultures.size(); i++)//loop through all cultures to find correct one
			if (cultures[i].id == id)
				return cultures[i];


		return cult;
	}
	Religion GameMapController::getReligionById(int id)
	{
		Religion rel;
		rel.id = -1;

		if (id >= 0 && id < religions.size())//if religion at index of id is correct religion return it
			if (religions[id].id == id)
				return religions[id];

		for (int i = 0; i < religions.size(); i++)//loop through all religions to find correct one
			if (religions[i].id == id)
				return religions[i];


		return rel;
	}

	Cell* GameMapController::getCellAtCoords(double xpos, double ypos)
	{
		float height = 0;

		for (int i = 0; i < cells.size(); i++)
		{
			int counter = 0;
			for (int j = 0, k = cells[i].coords.size() - 1; j < cells[i].coords.size(); k = j++)
			{
				if (Intersects(xpos, ypos, cells[i].coords[j].x, cells[i].coords[j].y, cells[i].coords[k].x, cells[i].coords[k].y))
				{
					counter++;
				}
			}
			if (counter % 2 != 0)
			{
				return &cells[i];
			}
		}
		//if cell is not found e.g. mouse if off screen return a nullptr
		return nullptr;
	}
	bool GameMapController::Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2)
	{

		if ((mouseY <= edgeY1) != (mouseY <= edgeY2))
		{
			if (mouseX <= (edgeX2 - edgeX1) * (mouseY - edgeY1) / (edgeY2 - edgeY1) + edgeX1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}