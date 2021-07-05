#pragma once
#include "Battle.h"
#include <vector>
#include <random>

namespace Mer
{
	class BattleController
	{
	public:
		BattleController();

		void NewBattle(Army* attacker, Army* defender, int biomeID);

		void Tick();
	private:
		std::vector<Battle> battles;

	};
}


