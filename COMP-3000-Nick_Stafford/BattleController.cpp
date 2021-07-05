#include "BattleController.h"
namespace Mer
{
	BattleController::BattleController()
	{
		
	}
	void BattleController::NewBattle(Army* attacker, Army* defender, int biomeID)
	{
		Battle temp;
		temp.attacker = attacker;
		temp.defender = defender;

		attacker->enganged = true;
		defender->enganged = true;

		switch (biomeID)
		{
		case 4:
			temp.attDisad == 0.95f;
			break;
		case 6:
			temp.attDisad == 0.90f;
			break;
		case 8:
			temp.attDisad == 0.85f;
			break;
		case 9:
			temp.attDisad == 0.85f;
			break;
		case 10:
			temp.attDisad == 0.75f;
			break;
		case 11:
			temp.attDisad == 0.80f;
			break;
		default:
			break;
		}

		temp.location = defender->locationID;

		battles.push_back(temp);
	}
	void BattleController::Tick(float dt, int gameSpeed)
	{
		battleAcc += dt;
		
		if (battleAcc >= (battleTimer / gameSpeed) / 4.0f)
		{
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(1, 6);
			for (int i = 0; i < battles.size(); i++)
			{
				int diceRoll = distribution(generator);

				if ((battles[i].attacker->attack * 0.4f) < battles[i].defender->defence)
				{
					battles[i].defender->morale -= ((battles[i].attacker->attack * battles[i].attDisad) * 0.6f) + diceRoll;
				}
				else
				{
					battles[i].defender->morale -= ((battles[i].attacker->attack * battles[i].attDisad) - battles[i].defender->defence) + diceRoll;
				}

				if (battles[i].defender->morale <= 0)
				{
					battles[i].attacker->enganged = false;
					battles[i].defender->enganged = false;
					battles[i].defender->broken = true;
					battles[i].over = true;
				}
				else
				{
					diceRoll = distribution(generator);
					if ((battles[i].defender->attack * 0.4f) < battles[i].defender->defence)
					{
						battles[i].attacker->morale -= ((battles[i].defender->attack) * 0.6f) + diceRoll;
					}
					else
					{
						battles[i].attacker->morale -= ((battles[i].defender->attack) - battles[i].attacker->defence) + diceRoll;
					}
				}

				if (battles[i].attacker->morale <= 0 && !battles[i].over)
				{
					battles[i].attacker->broken = true;
					battles[i].attacker->enganged = false;
					battles[i].defender->enganged = false;
					battles[i].over = true;
				}
				
			}

			for (int i = 0; i < battles.size(); i++)
			{
				if (battles[i].over)
				{
					battles.erase(battles.begin() + i);
				}
			}
			battleAcc = 0.0f;
		}


	}
}