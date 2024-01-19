#include "pch.h"
#include <random>
#include "Random.hpp"

// Handling randomness
std::mt19937 rng(time(NULL));
std::uniform_int_distribution<int> uniform_int_dist{ 0, 100 };
std::uniform_int_distribution<int> uniform_int_dist_below_5{ 0, 5 };
std::uniform_int_distribution<int> uniform_int_dist_above_5{ 5, 100 };
std::uniform_real_distribution<double> uniform_double_dist{ 0, 100 };
std::uniform_real_distribution<double> uniform_double_dist_below_5{ 0, 5 };
std::uniform_real_distribution<double> uniform_double_dist_above_5{ 5, 100 };

int getRandomInt() 
{ 
	return uniform_int_dist(rng); 
}

int getRandomIntBelow5() 
{
	return uniform_int_dist_below_5(rng);
}

int getRandomIntAbove5()
{
	return uniform_int_dist_above_5(rng);
}

double getRandomDouble() 
{ 
	return uniform_double_dist(rng); 
}

double getRandomDoubleBelow5()
{
	return uniform_double_dist_below_5(rng);
}

double getRandomDoubleAbove5()
{
	return uniform_double_dist_above_5(rng);
}