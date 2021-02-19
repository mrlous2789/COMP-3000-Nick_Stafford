#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"

namespace Mer
{
	struct River
	{
		std::vector<glm::vec3> coords;
		int id = -1;
		float width = 0.0f;
		float increment = 0.0f;
	};
}

