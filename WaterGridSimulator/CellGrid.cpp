#include "pch.h"
#include "CellGrid.hpp"

CellGrid::CellGrid(int numberRows, int numberColumns) noexcept
{
	for (int row = 0; row < numberRows; row++)
	{
		std::vector<Cell> cellRow;
		for (int column = 0; column < numberColumns; column++)
		{
			cellRow.push_back(Cell(0, 0, row, column));
		}
		m_cellGrid.push_back(cellRow);
	}
}

const Cell& CellGrid::getCell(int row, int column) const noexcept
{
	return m_cellGrid[row][column];
}

std::ostream& operator<<(std::ostream& os, const CellGrid& cellGrid)
{
	for (const std::vector<Cell>& cellRow : cellGrid.m_cellGrid)
	{
		for (const Cell& cell : cellRow)
		{
			os << cell << " ";
		}
		os << std::endl;
	}
	return os;
}
