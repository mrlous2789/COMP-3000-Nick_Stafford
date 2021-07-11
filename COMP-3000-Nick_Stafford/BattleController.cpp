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
			temp.attDisad == 0;
			break;
		case 6:
			temp.attDisad == 1;
			break;
		case 8:
			temp.attDisad == 2;
			break;
		case 9:
			temp.attDisad == 2;
			break;
		case 10:
			temp.attDisad == 4;
			break;
		case 11:
			temp.attDisad == 3;
			break;
		default:
			break;
		}

		temp.location = defender->locationID;

		battles.push_back(temp);
	}
	std::vector<Battle> BattleController::Tick(float dt, int gameSpeed)
	{
		std::vector<Battle> finishedBattles;

		battleAcc += dt;
		
		if (battleAcc >= (battleTimer / gameSpeed) / 4.0f)
		{
			std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
			std::uniform_int_distribution<int> distribution(1, 10);

			
			

			for (int i = 0; i < battles.size(); i++)
			{
				battles[i].ticks++;

				int diceRoll = distribution(generator);

				if (battles[i].attacker->bankrupt)
				{
					battles[i].defender->morale -= (((battles[i].attacker->attack * battles[i].attacker->attackMultiplier) * 0.5f) + diceRoll + battles[i].attacker->diceRollAdvantage);
				}
				else
				{
					battles[i].defender->morale -= (((battles[i].attacker->attack * battles[i].attacker->attackMultiplier)) + diceRoll + battles[i].attacker->diceRollAdvantage);
				}
				

				if (battles[i].defender->morale <= 0)
				{
					if (battles[i].ticks <= 10)
					{
						battles[i].warScore = 20.0f / battles[i].ticks;
					}
					else
					{
						battles[i].warScore = 2.0f;
					}
					battles[i].attacker->enganged = false;
					battles[i].defender->enganged = false;
					battles[i].defender->broken = true;
					battles[i].defender->morale = 100.0f;
					battles[i].attacker->morale = 100.0f;
					battles[i].over = true;
				}
				else
				{
					diceRoll = distribution(generator);
					if (battles[i].defender->bankrupt)
					{
						battles[i].attacker->morale -= ((battles[i].defender->attack * battles[i].defender->attackMultiplier) * 0.5f) + (diceRoll + battles[i].attDisad + battles[i].defender->diceRollAdvantage);
					}
					else
					{
						battles[i].attacker->morale -= ((battles[i].defender->attack * battles[i].defender->attackMultiplier)) + (diceRoll + battles[i].attDisad + battles[i].defender->diceRollAdvantage);
					}
					
				}

				if (battles[i].attacker->morale <= 0 && !battles[i].over)
				{
					if (battles[i].ticks <= 10)
					{
						battles[i].warScore = -20.0f / battles[i].ticks;
					}
					else
					{
						battles[i].warScore = -2.0f;
					}
					battles[i].attacker->broken = true;
					battles[i].attacker->enganged = false;
					battles[i].defender->enganged = false;
					battles[i].defender->morale = 100.0f;
					battles[i].attacker->morale = 100.0f;
					battles[i].over = true;
				}

				std::cout << "Attacker " << battles[i].attacker->nationID << " morale: " << battles[i].attacker->morale << " Defender " << battles[i].defender->nationID << " morale: " << battles[i].defender->morale << std::endl;
				
			}

			for (int i = 0; i < battles.size(); i++)
			{
				if (battles[i].over)
				{
					finishedBattles.push_back(battles[i]);
					battles.erase(battles.begin() + i);
				}
			}
			battleAcc = 0.0f;
		}


		return finishedBattles;

	}

	Battle* BattleController::getBattleInfoOf(int id)
	{
		for (int i = 0; i < battles.size(); i++)
		{
			if (battles[i].attacker->nationID == id || battles[i].defender->nationID == id)
			{
				return &battles[i];
			}
		}
		return nullptr;
	}

	int BattleController::getBattlesSize()
	{
		return battles.size();
	}
}