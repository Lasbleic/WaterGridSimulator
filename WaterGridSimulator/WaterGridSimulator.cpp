#include "pch.h"
#include "framework.h"
#include "WaterGridSimulator.hpp"

WaterGridSimulator::WaterGridSimulator(int numberRows, int numberColumns) noexcept
	: m_cellGrid(numberRows, numberColumns)
{
}

const CellGrid& WaterGridSimulator::getCellGrid() const noexcept
{
	return m_cellGrid;
}

void WaterGridSimulator::addWater(int row, int column, double volume) noexcept
{
	// TODO
}

void WaterGridSimulator::addFloor(int row, int column, int height) noexcept
{
	double volumeOfWaterReplaced = m_cellGrid.getCell(row, column).addFloor(height);
	addWater(row, column, volumeOfWaterReplaced);
}

Pond WaterGridSimulator::getPond(int initialRow, int initialColumn) noexcept
{
	assert(m_cellGrid.getCell(initialRow, initialColumn).hasWater() && "The pond should only be got from a water cell");

	CellSet waterCells;
	CellSet borderCells;
	std::unordered_set<std::pair<int, int>> visitedCellsPositions;
	std::unordered_set<std::pair<int, int>> cellsPositionsToVisit;
	//Add the pair of the coordinates of the cell to visit
	cellsPositionsToVisit.insert(std::pair<int, int>{initialRow, initialColumn});

	while (!cellsPositionsToVisit.empty())
	{
		std::pair<int, int> positionPair = *cellsPositionsToVisit.begin();
		cellsPositionsToVisit.erase(cellsPositionsToVisit.begin());
		visitedCellsPositions.insert(positionPair);

		int row = positionPair.first;
		int column = positionPair.second;
		Cell& cellToVisit = m_cellGrid.getCell(row, column);
		
		if (!cellToVisit.hasWater())
		{
			borderCells.insert(cellToVisit);
			continue;
		}

		waterCells.insert(cellToVisit);
		for (std::pair<int, int> positionPair : getNeighborCells(row, column))
		{
			if (visitedCellsPositions.find(positionPair) == visitedCellsPositions.end())
			{
				cellsPositionsToVisit.insert(positionPair);
			}
		}
	}

	return Pond{ waterCells, borderCells };
}

std::vector<std::pair<int, int>> WaterGridSimulator::getNeighborCells(int row, int column) noexcept
{
	std::vector<std::pair<int, int>> neighborCells;
	if (row > 0)
	{
		neighborCells.push_back(std::pair<int, int>{row - 1, column});
	}
	if (row < m_cellGrid.getNumberRows() - 1)
	{
		neighborCells.push_back(std::pair<int, int>{row + 1, column});
	}
	if (column > 0)
	{
		neighborCells.push_back(std::pair<int, int>{row, column - 1});
	}
	if (column < m_cellGrid.getNumberColumns() - 1)
	{
		neighborCells.push_back(std::pair<int, int>{row, column + 1});
	}
	return neighborCells;
}
