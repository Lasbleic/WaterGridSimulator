#include "pch.h"
#include "Pond.hpp"
#include "CellGridUtils.hpp"

Pond Pond::computePondFromCellWithWater(const CellPosition& cellPosition, const CellGrid& cellGrid)
{
	const Cell& initialCell = cellGrid.getCell(cellPosition);
	if (!initialCell.hasWater())
	{
		throw std::invalid_argument("A pond has sense only for water cells");
	}

	Pond pond{};

	CellPositionSet visitedCellsPositions;
	std::queue<CellPosition> cellsPositionsToVisit;
	cellsPositionsToVisit.push(cellPosition);

	// Initialize the lowest border cells floor level with the max value
	double lowestBorderCellsFloorLevel = std::numeric_limits<int>::max();

	while (!cellsPositionsToVisit.empty())
	{
		CellPosition currentCellPosition = cellsPositionsToVisit.front();
		cellsPositionsToVisit.pop();

		visitedCellsPositions.insert(currentCellPosition);

		const Cell& cellToVisit = cellGrid.getCell(currentCellPosition);

		// If cell is water cell, we keep exploring surrounding cells
		if (cellToVisit.hasWater())
		{
			pond.m_waterCells.insert(currentCellPosition);
			for (CellPosition neighbourPosition : getNeighborCellsPositions(currentCellPosition, cellGrid))
			{
				if (visitedCellsPositions.find(neighbourPosition) == visitedCellsPositions.end())
				{
					cellsPositionsToVisit.push(neighbourPosition);
				}
			}
			continue;
		}

		// If cell is not water cell, we add it to the border cells, and update the lowest border cells set by the way
		pond.m_borderCells.insert(currentCellPosition);

		// Create the unordered_set of the lowest border cells from the border cells
		if (cellToVisit.getLevel() == lowestBorderCellsFloorLevel)
		{
			pond.m_lowestBorderCells.insert(currentCellPosition);
		}
		else if (cellToVisit.getLevel() < lowestBorderCellsFloorLevel)
		{
			lowestBorderCellsFloorLevel = cellToVisit.getLevel();
			pond.m_lowestBorderCells.clear();
			pond.m_lowestBorderCells.insert(currentCellPosition);
		}
	}
	
	return pond;
}

std::size_t Pond::size() const noexcept
{
	// Return the number of water cells
	return m_waterCells.size();
}

const CellPositionSet& Pond::getLowestBorderCells() const noexcept
{
	return m_lowestBorderCells;
}

const CellPositionSet& Pond::getWaterCells() const noexcept
{
	return m_waterCells;
}