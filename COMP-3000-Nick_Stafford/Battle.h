#pragma once

#include "Army.h"

namespace Mer
{
	struct Battle
	{
		Army* attacker;
		Army* defender;

		int location;

		float attDisad = 0.0f;

		bool over = false;
	};
}