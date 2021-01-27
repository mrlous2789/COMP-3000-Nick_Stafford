#pragma once
#include "Reader.h"
namespace Mer
{
	class GameMapController
	{
	public:
		GameMapController();
		bool LoadFromFile(std::string location, std::string mapname);

		Cell* getCellAtCoords(double xpos, double ypos);

		std::vector<Cell> cells;
		std::vector<River> rivers;
		std::vector<Nation> nations;
		std::vector<Culture> cultures;
		std::vector<Religion> religions;

		Nation getNationById(int id);
		Culture getCultureById(int id);
		Religion getReligionById(int id);

	private:
		Reader reader;

		bool Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2);
	};
}


