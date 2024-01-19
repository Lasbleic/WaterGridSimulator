#include "pch.h"
#include "CppUnitTest.h"
#include "Random.hpp"
#include "../WaterGridSimulator/CellGrid.hpp"
#include "../WaterGridSimulator/CellPositionSet.hpp"
#include "../WaterGridSimulator/Pond.hpp"
#include "GridCreator.hpp"
#include "CellPositionSetHelper.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WaterGridSimulatorTests
{
	TEST_CLASS(PondTests)
	{
	public:
		
		TEST_METHOD(TestComputePondOnWaterLessCell)
		{
			// Creating lambda calling computePondFromCellWithWater on position (0, 0) of grid 1*1 with no water
			auto computePondFunction = [] { Pond::computePondFromCellWithWater(CellPosition{ 0, 0 }, CellGrid{1, 1}); };
			
			Assert::ExpectException<std::invalid_argument>(computePondFunction);
		}

		TEST_METHOD(TestComputePondOnSingleWaterCellLowerThanBorder) 
		{
			// Grid like:
			// F0  F1  F0
			// F1 W0.5 F1
			// F0  F1  F0
			CellGrid cellGrid{ 3, 3 };
			std::vector<CellPosition> borderPositions{
				CellPosition{0, 1},
				CellPosition{1, 0},
				CellPosition{1, 2},
				CellPosition{2, 1}
			};

			for (CellPosition borderPosition : borderPositions) 
			{
				cellGrid.getCell(borderPosition).addFloor(1);
			}
			cellGrid.getCell(CellPosition{1, 1}).addWater(0.5);

			Pond pond = Pond::computePondFromCellWithWater(CellPosition{ 1,1 }, cellGrid);

			Assert::AreEqual(pond.size(), static_cast<size_t>(1));
			Assert::AreEqual(pond.getLowestBorderCells().size(), static_cast<size_t>(4));
		}

		TEST_METHOD(TestComputePondOnSingleWaterCellEqualToBorder)
		{
			// Grid like:
			// F0 F1 F0
			// F1 W1 F1
			// F0 F1 F0
			CellGrid cellGrid{ 3, 3 };
			std::vector<CellPosition> borderPositions{
				CellPosition{0, 1},
				CellPosition{1, 0},
				CellPosition{1, 2},
				CellPosition{2, 1}
			};

			for (CellPosition borderPosition : borderPositions) 
			{
				cellGrid.getCell(borderPosition).addFloor(1);
			}
			cellGrid.getCell(CellPosition{ 1, 1 }).addWater(1);

			Pond pond = Pond::computePondFromCellWithWater(CellPosition{ 1,1 }, cellGrid);

			Assert::AreEqual(pond.size(), static_cast<size_t>(1));
			Assert::AreEqual(pond.getLowestBorderCells().size(), static_cast<size_t>(4));
		}

		TEST_METHOD(TestComputePondOnSingleWaterCellWithDifferentBorderHeight)
		{
			int NUMBER_LOWEST_CELL = getRandomInt(1, 4);

			CellGrid cellGrid{ 3, 3 };
			std::vector<CellPosition> borderPositions{
				CellPosition{0, 1},
				CellPosition{1, 0},
				CellPosition{1, 2},
				CellPosition{2, 1}
			};

			int numberLowestCellApplied = 0;
			for (CellPosition borderPosition : borderPositions) 
			{
				Cell& borderCell = cellGrid.getCell(borderPosition);
				if (numberLowestCellApplied < NUMBER_LOWEST_CELL) 
				{
					borderCell.addFloor(1);
					numberLowestCellApplied++;
				}
				else 
				{
					borderCell.addFloor(getRandomInt(2, 100));
				}
			}
			cellGrid.getCell(CellPosition{ 1, 1 }).addWater(1);

			Pond pond = Pond::computePondFromCellWithWater(CellPosition{ 1, 1 }, cellGrid);

			Assert::AreEqual(pond.size(), static_cast<size_t>(1));
			Assert::AreEqual(pond.getLowestBorderCells().size(), static_cast<size_t>(NUMBER_LOWEST_CELL));
		}

		TEST_METHOD(TestComputePondOnMultipleWaterCellSquarePond)
		{
			int GRID_NUMBER_ROW = getRandomInt(5, 15);
			int GRID_NUMBER_COLUMN = getRandomInt(5, 15);
			CellGrid cellGrid{ GRID_NUMBER_ROW, GRID_NUMBER_COLUMN };

			for (int i = 0; i < GRID_NUMBER_ROW; i++) 
			{
				for (int j = 0; j < GRID_NUMBER_COLUMN; j++)
				{
					if (i == 0 || i == GRID_NUMBER_ROW - 1 || j == 0 || j == GRID_NUMBER_COLUMN - 1) 
					{
						cellGrid.getCell(CellPosition{ i, j }).addFloor(1);
						continue;
					}

					cellGrid.getCell(CellPosition{ i, j }).addWater(0.5);
				}
			}

			Pond pond = Pond::computePondFromCellWithWater(CellPosition{ getRandomInt(1, GRID_NUMBER_ROW - 2), getRandomInt(1, GRID_NUMBER_COLUMN - 2) }, cellGrid);

			Assert::AreEqual(pond.size(), static_cast<size_t>((GRID_NUMBER_ROW - 2) * (GRID_NUMBER_COLUMN - 2)));
			Assert::AreEqual(pond.getLowestBorderCells().size(), static_cast<size_t>((GRID_NUMBER_ROW - 2) * 2 + (GRID_NUMBER_COLUMN - 2) * 2));

			Pond pond2 = Pond::computePondFromCellWithWater(CellPosition{ getRandomInt(1, GRID_NUMBER_ROW - 2), getRandomInt(1, GRID_NUMBER_COLUMN - 2) }, cellGrid);

			// Compare pond water cells, should be exactly the same
			Assert::AreEqual(pond.size(), pond2.size());
			Assert::IsTrue(areAllElementEquals(pond.getWaterCells(), pond2.getWaterCells()));
		}
	};
}
