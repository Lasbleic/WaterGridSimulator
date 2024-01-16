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
	CellPosition updatedCellPosition{ row, column };
	Cell& updatedCell = m_cellGrid.getCell(updatedCellPosition);
	bool hasUpdatedCellWater = updatedCell.hasWater();
	if (hasUpdatedCellWater)
	{
		Pond pond = getPond(updatedCellPosition);
		double pondWaterLevel = updatedCell.getWaterLevel();
		double pondLowestBorderLevel = m_cellGrid.getCell(*pond.getLowestBorderCells().begin()).getFloorLevel();

		double addedVolumeByCell = volume / pond.size();
		double overflowingVolume = 0;

		if (pondWaterLevel + addedVolumeByCell > pondLowestBorderLevel)
		{
			addedVolumeByCell = pondLowestBorderLevel - pondWaterLevel;
			overflowingVolume = volume - addedVolumeByCell * pond.size();
		}

		CellPositionSet waterCells = pond.getWaterCells();
		for (CellPosition waterCellPosition : waterCells) {
			Cell& cell = m_cellGrid.getCell(waterCellPosition);
			cell.addWater(addedVolumeByCell);
		}

		if (overflowingVolume > 0)
		{
			CellPositionSet lowestBorderCells = pond.getLowestBorderCells();
			for (CellPosition lowestBorderCellPosition : lowestBorderCells) {
				Cell& cell = m_cellGrid.getCell(lowestBorderCellPosition);
				cell.addWater(overflowingVolume / lowestBorderCells.size());
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
		CellPositionSet cellsPositionsToVisitNextStep{ updatedCellPosition };
		while (!cellsPositionsToVisitNextStep.empty() && cellsPositionToAddWaterTo.empty())
		{
			for (CellPosition cellPosition : cellsPositionsToVisitNextStep)
			{
				cellsPositionsToVisit.insert(cellPosition);
			}
			cellsPositionsToVisitNextStep.clear();

			for (CellPosition cellPosition : cellsPositionsToVisit)
			{
				int row = cellPosition.getRow();
				int column = cellPosition.getColumn();
				if (row < 0 || row >= m_cellGrid.getNumberRows() || column < 0 || column >= m_cellGrid.getNumberColumns())
				{
					// If the cell is out of the grid, we add it to the set of cells to add water to and continue
					cellsPositionToAddWaterTo.insert(cellPosition);
					continue;
				}
				
				Cell& cellToVisit = m_cellGrid.getCell(cellPosition);
				if (cellToVisit.getLevel() < updatedCell.getLevel())
				{
					cellsPositionToAddWaterTo.insert(cellPosition);
				}
				else
				{
					if (cellToVisit.getLevel() > updatedCell.getLevel())
					{
						// If the cell is higher than the updated cell, we don't need to visit its neighbors
						// as water won't flow through them
						continue;
					}
					for (CellPosition neighborPositionPair : getNeighborCells(cellPosition, true))
					{
						if (visitedCellsPositions.find(neighborPositionPair) == visitedCellsPositions.end())
						{
							cellsPositionsToVisitNextStep.insert(neighborPositionPair);
						}
					}
				}
				visitedCellsPositions.insert(cellPosition);
			}
			cellsPositionsToVisit.clear();

			if (!cellsPositionToAddWaterTo.empty())
			{
				for (CellPosition cellPosition : cellsPositionToAddWaterTo)
				{
					int row = cellPosition.getRow();
					int column = cellPosition.getColumn();
					if (row < 0 || row >= m_cellGrid.getNumberRows() || column < 0 || column >= m_cellGrid.getNumberColumns())
					{
						// If the cell is out of the grid, we don't add water to it
						// TODO: keep track of the volume of water that has been lost
						continue;
					}
					m_cellGrid.getCell(cellPosition).addWater(volume / cellsPositionToAddWaterTo.size());
				}
			}
		}

		// If no cell is lower than floor level, we fill the pond with water
		if (cellsPositionToAddWaterTo.empty())
		{
			// If we haven't found any cell that is lower than the updated cell. In that case,
			// we add the water to every visited cell
			for (CellPosition cellPosition : visitedCellsPositions)
			{
				m_cellGrid.getCell(cellPosition).addWater(volume / visitedCellsPositions.size());
			}
		}

	}
}

void WaterGridSimulator::addFloor(int row, int column, int height) noexcept
{
	CellPosition updatedCellPosition{ row, column };
	double volumeOfWaterReplaced = m_cellGrid.getCell(updatedCellPosition).addFloor(height);
	if (volumeOfWaterReplaced > 0) {
		addWater(row, column, volumeOfWaterReplaced);
	}
}

Pond WaterGridSimulator::getPond(const CellPosition& cellPosition) noexcept
{
	assert(m_cellGrid.getCell(cellPosition).hasWater() && "The pond should only be got from a water cell");

	CellPositionSet waterCells;
	CellPositionSet borderCells;
	CellPositionSet visitedCellsPositions;
	CellPositionSet cellsPositionsToVisit;
	//Add the pair of the coordinates of the cell to visit
	cellsPositionsToVisit.insert(cellPosition);

	while (!cellsPositionsToVisit.empty())
	{
		CellPosition currentCellPosition = *cellsPositionsToVisit.begin();
		cellsPositionsToVisit.erase(cellsPositionsToVisit.begin());
		visitedCellsPositions.insert(currentCellPosition);

		Cell& cellToVisit = m_cellGrid.getCell(currentCellPosition);
		
		if (!cellToVisit.hasWater())
		{
			borderCells.insert(currentCellPosition);
			continue;
		}

		waterCells.insert(currentCellPosition);
		for (CellPosition positionPair : getNeighborCells(currentCellPosition))
		{
			if (visitedCellsPositions.find(positionPair) == visitedCellsPositions.end())
			{
				cellsPositionsToVisit.insert(positionPair);
			}
		}
	}

	return Pond{ waterCells, borderCells, m_cellGrid };
}

std::vector<CellPosition> WaterGridSimulator::getNeighborCells(const CellPosition& cellPosition, bool includeOutOfBoundaries) noexcept
{
	int row = cellPosition.getRow();
	int column = cellPosition.getColumn();

	std::vector<CellPosition> neighborCells;

	if (row > 0 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{row - 1, column});
	}

	if (row < m_cellGrid.getNumberRows() - 1 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{row + 1, column});
	}

	if (column > 0 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{row, column - 1});
	}

	if (column < m_cellGrid.getNumberColumns() - 1 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{row, column + 1});
	}

	return neighborCells;
}
