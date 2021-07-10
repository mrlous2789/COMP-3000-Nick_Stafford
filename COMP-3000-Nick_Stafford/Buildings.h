#pragma once
#include<string>

namespace Mer
{
	struct Building
	{
		int id;
		std::string name;
		std::string effect;
		int baseAmount;
		int maxAmount;
		int cost;
		int amount;
	};
}