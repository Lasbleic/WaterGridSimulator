#include "pch.h"
#include "CellGridUtils.hpp"

std::vector<CellPosition> getNeighborCellsPositions(const CellPosition& cellPosition, const CellGrid& cellGrid, bool includeOutOfBoundaries) noexcept
{
	int row = cellPosition.getRow();
	int column = cellPosition.getColumn();

	std::vector<CellPosition> neighborCells;

	if (row > 0 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{ row - 1, column });
	}

	if (row < cellGrid.getNumberRows() - 1 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{ row + 1, column });
	}

	if (column > 0 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{ row, column - 1 });
	}

	if (column < cellGrid.getNumberColumns() - 1 || includeOutOfBoundaries)
	{
		neighborCells.push_back(CellPosition{ row, column + 1 });
	}

	return neighborCells;
}
