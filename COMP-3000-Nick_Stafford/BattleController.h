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

		void Tick(float dt, int gameSpeed);
	private:
		std::vector<Battle> battles;

		

		float battleTimer = 20.0f, battleAcc = 0.0f;
	};
}


