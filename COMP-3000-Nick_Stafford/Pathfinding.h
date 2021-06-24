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

		std::vector<Cell*> route;
		std::vector<Cell>* _cells;
		Cell* destination;
	};
}


