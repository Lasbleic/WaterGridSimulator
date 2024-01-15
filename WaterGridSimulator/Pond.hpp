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
	const CellSet& lowestBorderCells() const noexcept;
	bool contains(const Cell& cell) const noexcept;
	void onCellUpdate(const Cell& updatedCell) noexcept;


private:
	CellSet m_waterCells;
	CellSet m_borderCells;
	CellSet m_lowestBorderCells;
	bool isBorderedBy(const Cell& cell) const noexcept;
	void computeLowestBorderCells() noexcept;
};