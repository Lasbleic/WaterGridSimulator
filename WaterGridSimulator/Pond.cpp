#include "pch.h"
#include "Pond.hpp"

Pond::Pond(const CellSet& waterCells, const CellSet& borderCells)
	: m_waterCells{ waterCells }, m_borderCells{ borderCells }, m_lowestBorderCells{}
{
	// Check that the pond is not empty
	assert(waterCells.size() > 0 && "Pond is created without any water cells in it");
	
	// Check that all water cells are water cells and have the same water level
	double waterLevel = waterCells.begin()->getWaterLevel();
	for (const Cell& cell : waterCells)
	{
		assert(cell.getWaterLevel() == waterLevel && "Pond is created with water cells having different water levels");
		assert(cell.hasWater() && "Pond is created with non-water cells");
	}

	// Check that all border cells are water less cells and are higher than the water level
	int lowestBorderCellsFloorLevel = borderCells.begin()->getFloorLevel();
	for (const Cell& cell : borderCells)
	{
		assert(!cell.hasWater() && "Pond is created with water cells as border cells");
		assert(cell.getFloorLevel() > waterLevel && "Pond is created with border cells lower than the water level");

		// Create the unordered_set of the lowest border cells from the border cells
		if (cell.getFloorLevel() == lowestBorderCellsFloorLevel)
		{
			m_lowestBorderCells.insert(cell);
		}
		else if (cell.getFloorLevel() < lowestBorderCellsFloorLevel)
		{
			lowestBorderCellsFloorLevel = cell.getFloorLevel();
			m_lowestBorderCells.clear();
			m_lowestBorderCells.insert(cell);
		}
	}
}

int Pond::size() const noexcept
{
	// Return the number of water cells
	return static_cast<int>(m_waterCells.size());
}

double Pond::waterLevel() const noexcept
{
	return m_waterCells.begin()->getWaterLevel();
}

int Pond::lowestBorderCellsFloorLevel() const noexcept
{
	return m_lowestBorderCells.begin()->getFloorLevel();
}

const CellSet& Pond::lowestBorderCells() const noexcept
{
	return m_lowestBorderCells;
}

bool Pond::contains(const Cell& cell) const noexcept
{
	return m_waterCells.find(cell) != m_waterCells.end();
}