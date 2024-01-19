#pragma once

#include "CellGrid.hpp"
#include "Pond.hpp"

class WaterGridSimulator
{
public:
	//Constructors
	WaterGridSimulator() = delete;
	WaterGridSimulator(int numberRows, int numberColumns) noexcept;

	//Getters
	const CellGrid& getCellGrid() const noexcept;

	//Methods
	void addWater(int row, int column, double volume);
	void addFloor(int row, int column, int height);

private:
	void addWater(const CellPosition& updatedCellPosition, double volume);
	void addWaterOnWaterCell(const CellPosition& updatedCellPosition, double volume);
	void addWaterOnFloorCell(const CellPosition& updatedCellPosition, double volume);
	CellGrid m_cellGrid;
};