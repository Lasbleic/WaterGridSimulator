#include "pch.h"
#include "Cell.hpp"
#include "CellPositionSet.hpp"
#include "CellGrid.hpp"


class Pond
{
public:
	static Pond computePondFromCellWithWater(const CellPosition& cellPosition, const CellGrid& cellGrid);
	std::size_t size() const noexcept;
	const CellPositionSet& getLowestBorderCells() const noexcept;
	const CellPositionSet& getWaterCells() const noexcept;


private:
	Pond() = default;
	CellPositionSet m_waterCells;
	CellPositionSet m_borderCells;
	CellPositionSet m_lowestBorderCells;
};