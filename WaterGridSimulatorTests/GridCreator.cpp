#include "pch.h"
#include "GridCreator.hpp"

CellGrid createComplexRealCellGrid() 
{
	// Grid be like:
	// f9 f9 f9 f6 f5 f4 f5 f5 f3
	// f8 W6 f6 f9 W3 f3 f1 W1 f2
	// f9 f7 f8 W4 f4 f3 f6 W1 f1
	// f6 W5 W5 f5 W3 W3 f4 W1 f1
	// f7 W5 W5 W5 f6 W3 f3 W1 f2
	// f2 f8 W5 W5 f5 W3 f4 W1 f1
	// f0 f9 f6 f5 f2 f6 f0 f1 f0

	CellGrid cellGrid{ 7, 9 };

	// f9 f9 f9 f6 f5 f4 f5 f5 f3
	cellGrid.getCell(CellPosition{ 0, 0 }).addFloor(9);
	cellGrid.getCell(CellPosition{ 0, 1 }).addFloor(9);
	cellGrid.getCell(CellPosition{ 0, 2 }).addFloor(9);
	cellGrid.getCell(CellPosition{ 0, 3 }).addFloor(6);
	cellGrid.getCell(CellPosition{ 0, 4 }).addFloor(5);
	cellGrid.getCell(CellPosition{ 0, 5 }).addFloor(4);
	cellGrid.getCell(CellPosition{ 0, 6 }).addFloor(5);
	cellGrid.getCell(CellPosition{ 0, 7 }).addFloor(5);
	cellGrid.getCell(CellPosition{ 0, 8 }).addFloor(3);
	// f8 W6 f6 f9 W3 f3 f1 W1 f2
	cellGrid.getCell(CellPosition{ 1, 0 }).addFloor(8);
	cellGrid.getCell(CellPosition{ 1, 1 }).addWater(6);
	cellGrid.getCell(CellPosition{ 1, 2 }).addFloor(6);
	cellGrid.getCell(CellPosition{ 1, 3 }).addFloor(9);
	cellGrid.getCell(CellPosition{ 1, 4 }).addWater(3);
	cellGrid.getCell(CellPosition{ 1, 5 }).addFloor(3);
	cellGrid.getCell(CellPosition{ 1, 6 }).addFloor(1);
	cellGrid.getCell(CellPosition{ 1, 7 }).addWater(1);
	cellGrid.getCell(CellPosition{ 1, 8 }).addFloor(2);
	// f9 f7 f8 W4 f4 f3 f6 W1 f1
	cellGrid.getCell(CellPosition{ 2, 0 }).addFloor(9);
	cellGrid.getCell(CellPosition{ 2, 1 }).addFloor(7);
	cellGrid.getCell(CellPosition{ 2, 2 }).addFloor(8);
	cellGrid.getCell(CellPosition{ 2, 3 }).addWater(4);
	cellGrid.getCell(CellPosition{ 2, 4 }).addFloor(4);
	cellGrid.getCell(CellPosition{ 2, 5 }).addFloor(3);
	cellGrid.getCell(CellPosition{ 2, 6 }).addFloor(6);
	cellGrid.getCell(CellPosition{ 2, 7 }).addWater(1);
	cellGrid.getCell(CellPosition{ 2, 8 }).addFloor(1);
	// f6 W5 W5 f5 W3 W3 f4 W1 f1
	cellGrid.getCell(CellPosition{ 3, 0 }).addFloor(6);
	cellGrid.getCell(CellPosition{ 3, 1 }).addWater(5);
	cellGrid.getCell(CellPosition{ 3, 2 }).addWater(5);
	cellGrid.getCell(CellPosition{ 3, 3 }).addFloor(5);
	cellGrid.getCell(CellPosition{ 3, 4 }).addWater(3);
	cellGrid.getCell(CellPosition{ 3, 5 }).addWater(3);
	cellGrid.getCell(CellPosition{ 3, 6 }).addFloor(4);
	cellGrid.getCell(CellPosition{ 3, 7 }).addWater(1);
	cellGrid.getCell(CellPosition{ 3, 8 }).addFloor(1);
	// f7 W5 W5 W5 f6 W3 f3 W1 f2
	cellGrid.getCell(CellPosition{ 4, 0 }).addFloor(7);
	cellGrid.getCell(CellPosition{ 4, 1 }).addWater(5);
	cellGrid.getCell(CellPosition{ 4, 2 }).addWater(5);
	cellGrid.getCell(CellPosition{ 4, 3 }).addWater(5);
	cellGrid.getCell(CellPosition{ 4, 4 }).addFloor(6);
	cellGrid.getCell(CellPosition{ 4, 5 }).addWater(3);
	cellGrid.getCell(CellPosition{ 4, 6 }).addFloor(3);
	cellGrid.getCell(CellPosition{ 4, 7 }).addWater(1);
	cellGrid.getCell(CellPosition{ 4, 8 }).addFloor(2);
	// f2 f8 W5 W5 f5 W3 f4 W1 f1
	cellGrid.getCell(CellPosition{ 5, 0 }).addFloor(2);
	cellGrid.getCell(CellPosition{ 5, 1 }).addFloor(8);
	cellGrid.getCell(CellPosition{ 5, 2 }).addWater(5);
	cellGrid.getCell(CellPosition{ 5, 3 }).addWater(5);
	cellGrid.getCell(CellPosition{ 5, 4 }).addFloor(5);
	cellGrid.getCell(CellPosition{ 5, 5 }).addWater(3);
	cellGrid.getCell(CellPosition{ 5, 6 }).addFloor(4);
	cellGrid.getCell(CellPosition{ 5, 7 }).addWater(1);
	cellGrid.getCell(CellPosition{ 5, 8 }).addFloor(1);
	// f0 f9 f6 f5 f2 f6 f0 f1 f0
	cellGrid.getCell(CellPosition{ 6, 0 }).addFloor(0);
	cellGrid.getCell(CellPosition{ 6, 1 }).addFloor(9);
	cellGrid.getCell(CellPosition{ 6, 2 }).addFloor(6);
	cellGrid.getCell(CellPosition{ 6, 3 }).addFloor(5);
	cellGrid.getCell(CellPosition{ 6, 4 }).addFloor(2);
	cellGrid.getCell(CellPosition{ 6, 5 }).addFloor(6);
	cellGrid.getCell(CellPosition{ 6, 6 }).addFloor(0);
	cellGrid.getCell(CellPosition{ 6, 7 }).addFloor(1);
	cellGrid.getCell(CellPosition{ 6, 8 }).addFloor(0);

	return cellGrid;
}