#pragma once 
#include "glm/glm.hpp"
#include <vector>

namespace Mer
{
	struct ButtonElement
	{
		int bufferID = 1;
		std::string name;
		std::vector<glm::vec2> vertices;
		bool pressed = false;
		bool active = false;
	};
	struct PanelElement 
	{
		int bufferID = 1;
		std::string name;
		std::vector<glm::vec2> vertices;
		bool active = false;
	};
}
