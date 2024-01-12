#include "pch.h"
#include "Cell.hpp"
#include "CellSet.hpp"


class Pond
{
public:
	Pond(const CellSet& waterCells, const CellSet& borderCells);
	int size() const noexcept;
	double waterLevel() const noexcept;
	int lowestBorderCellsFloorLevel() const noexcept;
	CellSet lowestBorderCells() const noexcept;


private:
	CellSet m_waterCells;
	CellSet m_borderCells;
	CellSet m_lowestBorderCells;
};