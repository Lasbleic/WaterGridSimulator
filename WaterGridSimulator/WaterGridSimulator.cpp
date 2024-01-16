#include "pch.h"
#include "framework.h"
#include "WaterGridSimulator.hpp"

// Write a lambda to create a hash function for std::pair<int, int>

struct PairHasher
{
	std::size_t operator()(const std::pair<int, int>& pair) const noexcept
	{
		// Return the hash of the pair
		return std::hash<int>()(pair.first) ^ std::hash<int>()(pair.second) << 1;
	}
};
using CellPositionSet = std::unordered_set<std::pair<int, int>, PairHasher>;

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
	Cell& updatedCell = m_cellGrid.getCell(row, column);
	bool hasUpdatedCellWater = updatedCell.hasWater();
	if (hasUpdatedCellWater)
	{
		Pond pond = getPond(row, column);
		double addedVolumeByCell = volume / pond.size();
		double overflowingVolume = 0;
		if (pond.waterLevel() + addedVolumeByCell > pond.lowestBorderCellsFloorLevel())
		{
			addedVolumeByCell = pond.lowestBorderCellsFloorLevel() - pond.waterLevel();
			overflowingVolume = volume - addedVolumeByCell * pond.size();
		}

		CellSet waterCells = pond.getWaterCells();
		for (Cell cell : waterCells) {
			cell.addWater(addedVolumeByCell);
		}

		if (overflowingVolume > 0)
		{
			for (auto it = pond.lowestBorderCells().begin(); it != pond.lowestBorderCells().end(); ++it) {
				Cell cell = *it;
				cell.addWater(overflowingVolume / pond.lowestBorderCells().size());
			}
		}
	}
	else
	{
		//Add the water to the closest cells that level is lower than the updated cell
		CellPositionSet visitedCellsPositions;
		CellPositionSet cellsPositionsToVisit;
		CellPositionSet cellsPositionToAddWaterTo;
		// We separe the "cells to visit" in two sets to keep track of the distance from the updated cell
		CellPositionSet cellsPositionsToVisitNextStep{ std::pair<int, int>(row, column) };
		while (!cellsPositionsToVisitNextStep.empty() && cellsPositionToAddWaterTo.empty())
		{
			for (std::pair<int, int> positionPair : cellsPositionsToVisitNextStep)
			{
				cellsPositionsToVisit.insert(positionPair);
			}
			cellsPositionsToVisitNextStep.clear();

			for (std::pair<int, int> positionPair : cellsPositionsToVisit)
			{
				int row = positionPair.first;
				int column = positionPair.second;
				if (row < 0 || row >= m_cellGrid.getNumberRows() || column < 0 || column >= m_cellGrid.getNumberColumns())
				{
					// If the cell is out of the grid, we add it to the set of cells to add water to and continue
					cellsPositionToAddWaterTo.insert(positionPair);
					continue;
				}
				
				Cell& cellToVisit = m_cellGrid.getCell(row, column);
				if (cellToVisit.getLevel() < updatedCell.getLevel())
				{
					cellsPositionToAddWaterTo.insert(positionPair);
				}
				else
				{
					if (cellToVisit.getLevel() > updatedCell.getLevel())
					{
						// If the cell is higher than the updated cell, we don't need to visit its neighbors
						// as water won't flow through them
						continue;
					}
					for (std::pair<int, int> neighborPositionPair : getNeighborCells(row, column, true))
					{
						if (visitedCellsPositions.find(neighborPositionPair) == visitedCellsPositions.end())
						{
							cellsPositionsToVisitNextStep.insert(neighborPositionPair);
						}
					}
				}
				visitedCellsPositions.insert(positionPair);
			}
			cellsPositionsToVisit.clear();

			if (!cellsPositionToAddWaterTo.empty())
			{
				for (std::pair<int, int> positionPair : cellsPositionToAddWaterTo)
				{
					int row = positionPair.first;
					int column = positionPair.second;
					if (row < 0 || row >= m_cellGrid.getNumberRows() || column < 0 || column >= m_cellGrid.getNumberColumns())
					{
						// If the cell is out of the grid, we don't add water to it
						// TODO: keep track of the volume of water that has been lost
						continue;
					}
					m_cellGrid.getCell(row, column).addWater(volume / cellsPositionToAddWaterTo.size());
				}
			}
		}

		// If no cell is lower than floor level, we fill the pond with water
		if (cellsPositionToAddWaterTo.empty())
		{
			// If we haven't found any cell that is lower than the updated cell. In that case,
			// we add the water to every visited cell
			for (std::pair<int, int> positionPair : visitedCellsPositions)
			{
				int row = positionPair.first;
				int column = positionPair.second;
				m_cellGrid.getCell(row, column).addWater(volume / visitedCellsPositions.size());
			}
		}

	}
}

void WaterGridSimulator::addFloor(int row, int column, int height) noexcept
{
	double volumeOfWaterReplaced = m_cellGrid.getCell(row, column).addFloor(height);
	if (volumeOfWaterReplaced > 0) {
		addWater(row, column, volumeOfWaterReplaced);
	}
}

Pond WaterGridSimulator::getPond(int initialRow, int initialColumn) noexcept
{
	assert(m_cellGrid.getCell(initialRow, initialColumn).hasWater() && "The pond should only be got from a water cell");

	CellSet waterCells;
	CellSet borderCells;
	CellPositionSet visitedCellsPositions;
	CellPositionSet cellsPositionsToVisit;
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

std::vector<std::pair<int, int>> WaterGridSimulator::getNeighborCells(int row, int column, bool includeOutOfBoundaries) noexcept
{
	std::vector<std::pair<int, int>> neighborCells;
	if (row > 0 || includeOutOfBoundaries)
	{
		neighborCells.push_back(std::pair<int, int>{row - 1, column});
	}
	if (row < m_cellGrid.getNumberRows() - 1 || includeOutOfBoundaries)
	{
		neighborCells.push_back(std::pair<int, int>{row + 1, column});
	}
	if (column > 0 || includeOutOfBoundaries)
	{
		neighborCells.push_back(std::pair<int, int>{row, column - 1});
	}
	if (column < m_cellGrid.getNumberColumns() - 1 || includeOutOfBoundaries)
	{
		neighborCells.push_back(std::pair<int, int>{row, column + 1});
	}
	return neighborCells;
}
