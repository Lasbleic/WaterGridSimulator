#pragma once

#include "pch.h"
#include "Cell.hpp"

class CellGrid
{
public:
	//Constructors
	CellGrid() = delete;
	CellGrid(int numberRows, int numberColumns) noexcept;

	//Getters and Setters
	Cell& getCell(int row, int column) noexcept;
	int getNumberRows() const noexcept;
	int getNumberColumns() const noexcept;

	//Operators
	friend std::ostream& operator<<(std::ostream& os, const CellGrid& cellGrid);

private:
	std::vector<std::vector<Cell>> m_cellGrid;
};

