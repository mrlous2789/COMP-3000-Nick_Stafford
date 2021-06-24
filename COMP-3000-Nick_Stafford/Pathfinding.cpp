#include "Pathfinding.h"
namespace Mer
{
	Pathfinding::Pathfinding()
	{

	}

	void Pathfinding::Initialise(std::vector<Cell>* cells)
	{
		this->_cells = cells;
	}
	std::vector<Cell*> Pathfinding::CalculatePath(Cell* start, Cell* destination)
	{
		route.clear();
		route.push_back(start);
		this->destination = destination;

		bool routeFound = false;

		while (!routeFound)
		{
			double lowestF = 1000.0f;
			int lowestFID = 0;
			for (int i = 0; i < route.back()->neighbors.size(); i++)
			{
				double currentF = CalculateF(route.back()->neighbors[i].first, route.back()->neighbors[i].second);

				if (currentF < lowestF)
				{
					lowestF = currentF;
					lowestFID = route.back()->neighbors[i].first;
				}				
			}
			route.push_back(&_cells->at(lowestFID));

			if (route.back()->id == destination->id)
			{
				routeFound = true;
				std::cout << "woah no way this worked lol " << std::endl;
			}
		}


		return route;
	}

	double Pathfinding::CalculateF(int cellID, double g)
	{
		return (g + CalculateH(cellID));
	}

	double Pathfinding::CalculateH(int cellID)//movement cost from cell to destination
	{
		double xDistance = abs(_cells->at(cellID).centre.x - destination->centre.x);
		double yDistance = abs(_cells->at(cellID).centre.y - destination->centre.y);

		return xDistance + yDistance;
	}
}