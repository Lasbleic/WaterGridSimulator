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
