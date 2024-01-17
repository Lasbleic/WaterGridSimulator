#include "pch.h"
#include "Pond.hpp"

Pond::Pond(const CellPositionSet& waterCells, const CellPositionSet& borderCells, const CellGrid& cellGrid)
	: m_waterCells{ waterCells }, m_borderCells{ borderCells }, m_lowestBorderCells{}
{
	// Check that the pond is not empty
	assert(waterCells.size() > 0 && "Pond is created without any water cells in it");
	
	// Check that all water cells are water cells and have the same water level
	double waterLevel = cellGrid.getCell(*waterCells.begin()).getLevel();

	for (CellPosition cellPosition : waterCells)
	{
		assert(cellGrid.getCell(cellPosition).getLevel() == waterLevel && "Pond is created with water cells having different water levels");
		assert(cellGrid.getCell(cellPosition).hasWater() && "Pond is created with non-water cells");
	}

	// Check that all border cells are water less cells and are higher than the water level
	double lowestBorderCellsFloorLevel = cellGrid.getCell(*borderCells.begin()).getLevel();
	for (CellPosition cellPosition : borderCells)
	{
		Cell cell = cellGrid.getCell(cellPosition);
		assert(!cell.hasWater() && "Pond is created with water cells as border cells");
		assert(cell.getLevel() >= waterLevel && "Pond is created with border cells lower than the water level");

		// Create the unordered_set of the lowest border cells from the border cells
		if (cell.getLevel() == lowestBorderCellsFloorLevel)
		{
			m_lowestBorderCells.insert(cellPosition);
		}
		else if (cell.getLevel() < lowestBorderCellsFloorLevel)
		{
			lowestBorderCellsFloorLevel = cell.getLevel();
			m_lowestBorderCells.clear();
			m_lowestBorderCells.insert(cellPosition);
		}
	}
}

int Pond::size() const noexcept
{
	// Return the number of water cells
	return static_cast<int>(m_waterCells.size());
}

const CellPositionSet& Pond::getLowestBorderCells() const noexcept
{
	return m_lowestBorderCells;
}

const CellPositionSet& Pond::getWaterCells() const noexcept
{
	return m_waterCells;
}