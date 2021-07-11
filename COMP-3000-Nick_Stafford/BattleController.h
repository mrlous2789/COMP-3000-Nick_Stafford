#pragma once
#include "Battle.h"
#include <vector>
#include <random>
#include <iostream>
#include <chrono>

namespace Mer
{
	class BattleController
	{
	public:
		BattleController();

		void NewBattle(Army* attacker, Army* defender, int biomeID);

		std::vector<Battle> Tick(float dt, int gameSpeed);

		Battle* getBattleInfoOf(int id);

		int getBattlesSize();

		
	private:
		std::vector<Battle> battles;

		

		float battleTimer = 20.0f, battleAcc = 0.0f;
	};
}


