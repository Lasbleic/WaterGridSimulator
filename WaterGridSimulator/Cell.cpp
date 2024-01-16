#include "pch.h"
#include "Cell.hpp"

// Constructors
Cell::Cell(int floorLevel, double waterLevel, int row, int column) noexcept
	: m_floorLevel(floorLevel), m_waterLevel(waterLevel), m_cellPosition(row, column)
{
}

// Getters and Setters
const CellPosition& Cell::getCellPosition() const noexcept
{
	return m_cellPosition;
}

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

int Cell::getFloorLevel() const noexcept
{
	return m_floorLevel;
}

double Cell::waterVolume() const noexcept
{
	return std::max(0.0, m_waterLevel - m_floorLevel);
}

double Cell::addFloor(int floorHeight) noexcept
{
	assert(floorHeight >= 0 && "floorHeight must be positive");
	// Expliciting the link between the floor height added and the volume, even if it's equal
	double floorVolumeAdded = floorHeight * 1.0 * 1.0;
	double volumeOfWaterReplaced = std::min(waterVolume(), floorVolumeAdded);
	m_floorLevel += floorHeight;
	return volumeOfWaterReplaced;
}

void Cell::addWater(double waterVolume) noexcept
{
	if (hasWater())
	{
		m_waterLevel += waterVolume;
	}
	else
	{
		m_waterLevel = m_floorLevel + waterVolume;
	}
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

bool operator==(const Cell& cell1, const Cell& cell2)
{
	return cell1.m_cellPosition == cell2.m_cellPosition;
}

bool operator!=(const Cell& cell1, const Cell& cell2)
{
	return !(cell1 == cell2);
}