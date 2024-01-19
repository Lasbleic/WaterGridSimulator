#include "pch.h"
#include "framework.h"
#include "WaterGridSimulator.hpp"
#include "CellGridUtils.hpp"

WaterGridSimulator::WaterGridSimulator(int numberRows, int numberColumns) noexcept
	: m_cellGrid(numberRows, numberColumns)
{
}

const CellGrid& WaterGridSimulator::getCellGrid() const noexcept
{
	return m_cellGrid;
}

void WaterGridSimulator::addWater(int row, int column, double volume)
{
	addWater(CellPosition{ row, column }, volume);
}

void WaterGridSimulator::addWater(const CellPosition& updatedCellPosition, double volume)
{
	Cell& updatedCell = m_cellGrid.getCell(updatedCellPosition);
	if (updatedCell.hasWater())
	{
		addWaterOnWaterCell(updatedCellPosition, volume);
	}
	else
	{
		addWaterOnFloorCell(updatedCellPosition, volume);
	}
}

void WaterGridSimulator::addWaterOnWaterCell(const CellPosition& updatedCellPosition, double volume)
{
	Cell& updatedCell = m_cellGrid.getCell(updatedCellPosition);
	
	Pond pond = Pond::computePondFromCellWithWater(updatedCellPosition, m_cellGrid);
	double pondWaterLevel = updatedCell.getLevel();
	double pondLowestBorderLevel = m_cellGrid.getCell(*pond.getLowestBorderCells().begin()).getLevel();

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
			addWater(lowestBorderCellPosition, overflowingVolume / lowestBorderCells.size());
		}
	}
}

void WaterGridSimulator::addWaterOnFloorCell(const CellPosition& updatedCellPosition, double volume)
{
	Cell& updatedCell = m_cellGrid.getCell(updatedCellPosition);

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
			if (isPositionOutOfGrid(cellPosition, m_cellGrid))
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
				for (CellPosition neighborPositionPair : getNeighborCellsPositions(cellPosition, m_cellGrid, true))
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
				if (isPositionOutOfGrid(cellPosition, m_cellGrid))
				{
					// If the cell is out of the grid, we don't add water to it
					// TODO: keep track of the volume of water that has been lost
					continue;
				}
				addWater(cellPosition, volume / cellsPositionToAddWaterTo.size());
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

void WaterGridSimulator::addFloor(int row, int column, int height)
{
	CellPosition updatedCellPosition{ row, column };
	double volumeOfWaterReplaced = m_cellGrid.getCell(updatedCellPosition).addFloor(height);
	if (volumeOfWaterReplaced > 0) {
		addWater(updatedCellPosition, volumeOfWaterReplaced);
	}
}
