#include "pch.h"
#include "CellPosition.hpp"

CellPosition::CellPosition(int row, int column) noexcept
	: m_row(row), m_column(column)
{
}

std::size_t CellPosition::getHash() const noexcept
{
	size_t rowHash = std::hash<int>()(m_row);
	size_t colHash = std::hash<int>()(m_column) << 1;
	return rowHash ^ colHash;
}

bool operator==(const CellPosition& cellPosition1, const CellPosition& cellPosition2)
{
	return cellPosition1.m_row == cellPosition2.m_row && cellPosition1.m_column == cellPosition2.m_column;
}

bool operator!=(const CellPosition& cellPosition1, const CellPosition& cellPosition2)
{
	return !(cellPosition1 == cellPosition2);
}