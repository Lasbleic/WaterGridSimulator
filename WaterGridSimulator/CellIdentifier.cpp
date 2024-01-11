#include "pch.h"
#include "CellIdentifier.hpp"

CellIdentifier::CellIdentifier(int row, int column) noexcept
	: m_row(row), m_column(column)
{
}

bool operator==(const CellIdentifier& cellIdentifier1, const CellIdentifier& cellIdentifier2)
{
	return cellIdentifier1.m_row == cellIdentifier2.m_row && cellIdentifier1.m_column == cellIdentifier2.m_column;
}

bool operator!=(const CellIdentifier& cellIdentifier1, const CellIdentifier& cellIdentifier2)
{
	return !(cellIdentifier1 == cellIdentifier2);
}