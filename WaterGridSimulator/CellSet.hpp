#pragma once

#include "pch.h"
#include "Cell.hpp"

struct CellHasher
{
	std::size_t operator()(const Cell& cell) const noexcept
	{
		// Return the hash of the cell identifier
		return cell.getCellIdentifier().getIdentifierHash();
	}
};

using CellSet = std::unordered_set<Cell, CellHasher>;