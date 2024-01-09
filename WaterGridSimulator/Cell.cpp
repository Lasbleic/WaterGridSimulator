#include "pch.h"
#include "Cell.hpp"

// Constructors
Cell::Cell(int floorLevel, double waterLevel = 0) noexcept
	: m_floorLevel(floorLevel), m_waterLevel(waterLevel)
{
}

// Getters and Setters
bool Cell::hasWater() const noexcept
{
	return m_waterLevel > m_floorLevel;
}

double Cell::getLevel() const noexcept
{
	if (hasWater())
	{
		return m_waterLevel;
	}
	else
	{
		return m_floorLevel;
	}
}

double Cell::getWaterLevel() const noexcept
{
	return m_waterLevel;
}

void Cell::setWaterLevel(double waterLevel) noexcept
{
	m_waterLevel = waterLevel;
}

int Cell::getFloorLevel() const noexcept
{
	return m_floorLevel;
}

void Cell::setFloorLevel(int floorLevel) noexcept
{
	m_floorLevel = floorLevel;
}

// Operators
std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
	if (cell.hasWater()) 
	{
		// Print the water level in blue with a precision of 1 decimal
		os << "\033[36m" << std::fixed << std::setprecision(1) << cell.m_waterLevel << "\033[0m";
	}
	else 
	{
		os << " " << cell.m_floorLevel << " ";
	}
	return os;
}