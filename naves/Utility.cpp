#include "Utility.h"

int rndNum(int min, int max)
{
	std::random_device rDev;
	std::mt19937 mt(rDev());

	std::uniform_int_distribution<int> rng(min, max);
	return rng(mt);
}
