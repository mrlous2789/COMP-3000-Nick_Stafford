#pragma once
#include <string>
#include "Cell.h"
//data structure for a nation
namespace Mer
{
	struct Nation
	{
		int id;
		std::string name = "big country";
		std::vector<Cell*> nationCells;
		int capitalId;
		Cell* capital;
		float colour[3];


	};
}
