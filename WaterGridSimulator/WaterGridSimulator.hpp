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
	void addWater(int row, int column, double volume) noexcept;
	void addFloor(int row, int column, int height) noexcept;

private:
	CellGrid m_cellGrid;
	Pond getPond(int initialRow, int initialColumn) noexcept;
	std::vector<std::pair<int, int>> getNeighborCells(int row, int column) noexcept;
};