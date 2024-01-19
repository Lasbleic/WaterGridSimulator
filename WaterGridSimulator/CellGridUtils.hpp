#pragma once

#include "pch.h"
#include "CellPosition.hpp"
#include "CellGrid.hpp"

std::vector<CellPosition> getNeighborCellsPositions(const CellPosition& cellPosition, const CellGrid& cellGrid, bool includeOutOfBoundaries = false) noexcept;

bool isPositionOutOfGrid(const CellPosition& cellPosition, const CellGrid& cellGrid);