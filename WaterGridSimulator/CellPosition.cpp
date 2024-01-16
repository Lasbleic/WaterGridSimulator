#include "pch.h"
#include "CellPosition.hpp"

CellPosition::CellPosition(int row, int column) noexcept
	: m_row(row), m_column(column)
{
}

int CellPosition::getRow() const noexcept
{
	return m_row;
}

int CellPosition::getColumn() const noexcept
{
	return m_column;
}

bool operator==(const CellPosition& cellPosition1, const CellPosition& cellPosition2)
{
	return cellPosition1.m_row == cellPosition2.m_row && cellPosition1.m_column == cellPosition2.m_column;
}

bool operator!=(const CellPosition& cellPosition1, const CellPosition& cellPosition2)
{
	return !(cellPosition1 == cellPosition2);
}

size_t CellPosition::CellPositionHasher::operator()(const CellPosition& cellPosition) const
{
	size_t rowHash = std::hash<int>()(cellPosition.m_row);
	size_t colHash = std::hash<int>()(cellPosition.m_column) << 1;
	return rowHash ^ colHash;
}

