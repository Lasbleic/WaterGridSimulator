#include "pch.h"
#include "CellPositionSetHelper.hpp"

bool areAllElementEquals(const CellPositionSet& cellPositionSet1, const CellPositionSet& cellPositionSet2)
{
	bool hasFoundElementNotIdentical = false;

	for (CellPosition pondPosition : cellPositionSet1) {

		// If there is an element on pond.waterCells which is not in pond2.waterCells
		if (cellPositionSet2.find(pondPosition) == cellPositionSet2.end()) {
			hasFoundElementNotIdentical = true;
			break;
		}
	}

	return !hasFoundElementNotIdentical;
}