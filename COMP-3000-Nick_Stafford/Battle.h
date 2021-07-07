#pragma once

#include "Army.h"
#include "Wars.h"

namespace Mer
{
	struct Battle
	{
		Army* attacker;
		Army* defender;

		float warScore;

		int location;

		int attDisad = 0;
		int ticks = 0;

		bool over = false;
	};
}