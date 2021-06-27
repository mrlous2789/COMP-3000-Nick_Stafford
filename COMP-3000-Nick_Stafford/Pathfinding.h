#pragma once


#include "Cell.h"
#include <vector>

namespace Mer
{
	class Pathfinding
	{
	public:
		Pathfinding();

		void Initialise(std::vector<Cell>* _cells);

		std::vector<Cell*> CalculatePath(Cell* start, Cell* destination);

	private:
		double CalculateF(int cellID, double g);
		double CalculateH(int cellID);

		bool IsInRoute(int id);

		std::vector<Cell*> route;
		std::vector<Cell>* _cells;
		Cell* destination;
	};
}


