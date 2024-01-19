#include "pch.h"
#include "framework.h"
#include "WaterGridSimulator.hpp"
#include "CellGridUtils.hpp"

WaterGridSimulator::WaterGridSimulator(int numberRows, int numberColumns) noexcept
	: m_cellGrid(numberRows, numberColumns), m_waterVolumeLost(0)
{
}

const CellGrid& WaterGridSimulator::getCellGrid() const noexcept
{
	return m_cellGrid;
}

double WaterGridSimulator::getWaterVolumeLost() const noexcept 
{
	return m_waterVolumeLost;
}

void WaterGridSimulator::addWater(int row, int column, double volume)
{
	addWater(CellPosition{ row, column }, volume);
}

void WaterGridSimulator::addFloor(int row, int column, int height)
{
	CellPosition updatedCellPosition{ row, column };
	double volumeOfWaterReplaced = m_cellGrid.getCell(updatedCellPosition).addFloor(height);
	if (volumeOfWaterReplaced > 0) {
		addWater(updatedCellPosition, volumeOfWaterReplaced);
	}
}

void WaterGridSimulator::addWater(const CellPosition& updatedCellPosition, double volume)
{
	if (isPositionOutOfGrid(updatedCellPosition, m_cellGrid)) {
		m_waterVolumeLost += volume;
		return;
	}

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

	for (CellPosition waterCellPosition : pond.getWaterCells()) {
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
	CellPositionSet cellsPositionToAddWaterTo;
	std::list<CellPosition> cellsPositionsToVisit;
	// We separe the "cells to visit" in two sets to keep track of the distance from the updated cell
	std::list<CellPosition> cellsPositionsToVisitNextStep{ updatedCellPosition };
	
	bool lowerCellsToFlowToFound = false;

	// While we have cells to visit on next step and no cells to add water to, we keep visiting surrounding cells
	while (!cellsPositionsToVisitNextStep.empty() && !lowerCellsToFlowToFound)
	{
		// We transfer the cells positions to visit next step to the actual cells we will visit on this step
		cellsPositionsToVisit.clear();
		for (CellPosition cellPosition : cellsPositionsToVisitNextStep)
		{
			cellsPositionsToVisit.push_back(cellPosition);
		}
		cellsPositionsToVisitNextStep.clear();

		for (CellPosition cellPosition : cellsPositionsToVisit)
		{
			if (isPositionOutOfGrid(cellPosition, m_cellGrid))
			{
				// If the cell is out of the grid, we add it to the set of cells to add water to and continue
				cellsPositionToAddWaterTo.insert(cellPosition);
				lowerCellsToFlowToFound = true;
				continue;
			}

			Cell& cellToVisit = m_cellGrid.getCell(cellPosition);

			if (cellToVisit.getLevel() > updatedCell.getLevel())
			{
				// If the cell is higher than the updated cell, we don't need to visit its neighbors
				// as water won't flow through it
				continue;
			}

			if (cellToVisit.getLevel() < updatedCell.getLevel())
			{
				// The water will flow to this cell. We keep the "for" loop going on to check if there
				// would be other cells to flow to.
				cellsPositionToAddWaterTo.insert(cellPosition);
				continue;
			}

			for (CellPosition neighborPositionPair : getNeighborCellsPositions(cellPosition, m_cellGrid, true))
			{
				// If the neighbour has already been visited, we don't add it to cellsPositionToVisit
				if (visitedCellsPositions.find(neighborPositionPair) != visitedCellsPositions.end())
				{
					continue;
				}

				cellsPositionsToVisitNextStep.push_back(neighborPositionPair);
			}

			visitedCellsPositions.insert(cellPosition);
		}
	}

	if (lowerCellsToFlowToFound)
	{
		for (CellPosition cellPosition : cellsPositionToAddWaterTo)
		{
			addWater(cellPosition, volume / cellsPositionToAddWaterTo.size());
		}
	}
	else
	{
		// In that case, we add the water to every visited cell as it constitue a new pond
		for (CellPosition cellPosition : visitedCellsPositions)
		{
			m_cellGrid.getCell(cellPosition).addWater(volume / visitedCellsPositions.size());
		}
	}
}
