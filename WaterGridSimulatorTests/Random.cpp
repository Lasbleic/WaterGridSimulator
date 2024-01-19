#include "pch.h"
#include <random>
#include "Random.hpp"

// Handling randomness
std::mt19937 rng(time(NULL));
std::uniform_int_distribution<int> uniform_int_dist{ 1, 100 };
std::uniform_real_distribution<double> uniform_double_dist{ 1, 100 };

int getRandomInt() 
{ 
	return uniform_int_dist(rng); 
}

int getRandomInt(int min, int max) 
{
	return std::uniform_int_distribution<int>{ min, max }(rng);
}

double getRandomDouble() 
{ 
	return uniform_double_dist(rng); 
}

double getRandomDouble(double min, double max)
{
	return std::uniform_real_distribution<double>{ min, max }(rng);
}