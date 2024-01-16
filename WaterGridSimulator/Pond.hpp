#include "pch.h"
#include "Cell.hpp"
#include "CellPositionSet.hpp"
#include "CellGrid.hpp"


class Pond
{
public:
	Pond(const CellPositionSet& waterCells, const CellPositionSet& borderCells, const CellGrid& cellGrid);
	int size() const noexcept;
	const CellPositionSet& getLowestBorderCells() const noexcept;
	const CellPositionSet& getWaterCells() const noexcept;


private:
	CellPositionSet m_waterCells;
	CellPositionSet m_borderCells;
	CellPositionSet m_lowestBorderCells;
};