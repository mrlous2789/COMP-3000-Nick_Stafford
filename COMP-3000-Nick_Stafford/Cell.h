#pragma once
#include <iostream>
#include <vector>
#include "Army.h"

#include "glm/glm.hpp"



namespace Mer
{
	struct Cell
	{
		std::vector<glm::vec3> coords;//coords of the cells vertices
		int id = -1;//cell id
		float height = 0;//hieght of cell
		int biome = -1;//biome id
		std::string type;//type of cells e.g. ocean or land
		int population = 0;//cells population
		int state = -1;//cell nation state id 
		int province = -1;//cell province
		int culture = -1;//cell culture
		int religion = -1;//cell religion
		std::vector<std::pair<int,double>> neighbors;//neighboring cell ids
		glm::vec2 centre;

		Army* occupier = nullptr;
		bool isInPeaceDeal = false;
	};
}
