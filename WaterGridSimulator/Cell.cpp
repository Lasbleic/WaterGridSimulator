#include "pch.h"
#include "Cell.hpp"

// Constructors
Cell::Cell(int floorLevel, double waterVolume, int row, int column) noexcept
	: m_floorLevel(floorLevel), m_waterVolume(waterVolume), m_cellPosition(row, column)
{
}

// Getters and Setters
const CellPosition& Cell::getCellPosition() const noexcept
{
	return m_cellPosition;
}

bool Cell::hasWater() const noexcept
{
	return m_waterVolume > 0;
}

double Cell::getLevel() const noexcept
{
	return m_floorLevel + m_waterVolume;
}

double Cell::addFloor(int floorHeight) noexcept
{
	assert(floorHeight >= 0 && "floorHeight must be positive");
	// Expliciting the link between the floor height added and the volume, even if it's equal
	double floorVolumeAdded = floorHeight * 1.0 * 1.0;
	double volumeOfWaterReplaced = std::min(m_waterVolume, floorVolumeAdded);
	m_floorLevel += floorHeight;
	m_waterVolume -= volumeOfWaterReplaced;
	return volumeOfWaterReplaced;
}

void Cell::addWater(double waterVolume) noexcept
{
	m_waterVolume += waterVolume;
}

// Operators
std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
	if (cell.hasWater()) 
	{
		// Print the water level in blue with a precision of 1 decimal
		os << "\033[36m" << std::fixed << std::setprecision(1) << cell.getLevel() << "\033[0m";
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