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

		TEST_METHOD(TestComputePondOnRealComplexSituation)
		{
			// Grid be like:
			// f9 f9 f9 f6 f5 f4 f5 f5 f3
			// f8 W6 f6 f9 W3 f3 f1 W1 f2
			// f9 f7 f8 W4 f4 f3 f6 W1 f1
			// f6 W5 W5 f5 W3 W3 f4 W1 f1
			// f7 W5 W5 W5 f6 W3 f3 W1 f2
			// f2 f8 W5 W5 f5 W3 f4 W1 f1
			// f0 f9 f6 f5 f2 f6 f0 f1 f0

			CellGrid cellGrid = createComplexRealCellGrid();

			Pond highestPond = Pond::computePondFromCellWithWater(CellPosition{ 1, 1 }, cellGrid);
			Assert::AreEqual(highestPond.size(), static_cast<std::size_t>(1));
			Assert::AreEqual(highestPond.getLowestBorderCells().size(), static_cast<std::size_t>(1));

			Pond biggestPond1 = Pond::computePondFromCellWithWater(CellPosition{ 3, 1 }, cellGrid);
			Pond biggestPond2 = Pond::computePondFromCellWithWater(CellPosition{ 5, 3 }, cellGrid);
			Assert::AreEqual(biggestPond1.size(), static_cast<size_t>(7));
			Assert::IsTrue(areAllElementEquals(biggestPond1.getWaterCells(), biggestPond2.getWaterCells()));
			Assert::IsTrue(areAllElementEquals(biggestPond1.getLowestBorderCells(), biggestPond2.getLowestBorderCells()));
			Assert::IsTrue(areAllElementEquals(biggestPond1.getLowestBorderCells(), CellPositionSet{ CellPosition{ 3, 3 }, CellPosition{ 5, 4 }, CellPosition{ 6, 3 } }));

			Pond lonelyPond = Pond::computePondFromCellWithWater(CellPosition{ 2, 3 }, cellGrid);
			Assert::AreEqual(lonelyPond.size(), static_cast<std::size_t>(1));
			Assert::IsTrue(areAllElementEquals(lonelyPond.getLowestBorderCells(), CellPositionSet{ CellPosition{ 2, 4 } }));
		}
	};
}
