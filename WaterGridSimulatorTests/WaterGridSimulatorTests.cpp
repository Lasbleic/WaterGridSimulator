#include "pch.h"
#include "CppUnitTest.h"
#include "../WaterGridSimulator/Cell.hpp"
#include "Random.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WaterGridSimulatorTests
{
	TEST_CLASS(CellTests)
	{
	public:
		
		TEST_METHOD(TestHasWater)
		{
			int BASE_FLOOR_LEVEL = getRandomInt();
			double BASE_WATER_VOLUME = getRandomDouble();
			Cell waterLessCell{ BASE_FLOOR_LEVEL, 0, 0, 0 };
			Cell cellWithWater{ BASE_FLOOR_LEVEL, BASE_WATER_VOLUME, 0, 0 };

			Assert::IsFalse(waterLessCell.hasWater());
			Assert::IsTrue(cellWithWater.hasWater());
		}

		TEST_METHOD(TestGetLevel)
		{
			const int BASE_FLOOR_LEVEL = getRandomInt();
			const double BASE_WATER_VOLUME = getRandomDouble();
			Cell waterLessCell{ BASE_FLOOR_LEVEL, 0, 0, 0 };
			Cell cellWithWater{ BASE_FLOOR_LEVEL, BASE_WATER_VOLUME, 0, 0 };

			Assert::AreEqual(waterLessCell.getLevel(), static_cast<double>(BASE_FLOOR_LEVEL));
			Assert::AreEqual(cellWithWater.getLevel(), BASE_FLOOR_LEVEL + BASE_WATER_VOLUME);
		}

		TEST_METHOD(TestAddWater)
		{
			const int BASE_FLOOR_LEVEL = getRandomInt();
			const double BASE_WATER_VOLUME = getRandomDouble();
			const double WATER_VOLUME_TO_ADD = getRandomDouble();
			Cell waterLessCell{ BASE_FLOOR_LEVEL, 0, 0, 0 };
			Cell cellWithWater{ BASE_FLOOR_LEVEL, BASE_WATER_VOLUME, 0, 0 };


			waterLessCell.addWater(WATER_VOLUME_TO_ADD);
			cellWithWater.addWater(WATER_VOLUME_TO_ADD);

			Assert::AreEqual(waterLessCell.getLevel(), BASE_FLOOR_LEVEL + WATER_VOLUME_TO_ADD);
			Assert::AreEqual(cellWithWater.getLevel(), BASE_FLOOR_LEVEL + BASE_WATER_VOLUME + WATER_VOLUME_TO_ADD);
		}

		TEST_METHOD(TestAddFloor)
		{
			const int BASE_FLOOR_LEVEL = getRandomInt();
			const double BASE_WATER_VOLUME_BELOW_5 = getRandomDoubleBelow5();
			const double BASE_WATER_VOLUME_ABOVE_5 = getRandomDoubleAbove5();
			const int FLOOR_LEVEL_TO_ADD_BELOW_5 = getRandomIntBelow5();
			const int FLOOR_LEVEL_TO_ADD_ABOVE_5 = getRandomIntAbove5();

			Cell waterLessCell{ BASE_FLOOR_LEVEL, 0, 0, 0 };
			Cell cellWithWaterHigherThanFloorAdded{ BASE_FLOOR_LEVEL, BASE_WATER_VOLUME_ABOVE_5, 0, 0 };
			Cell cellWithWaterLowerThanFloorAdded{ BASE_FLOOR_LEVEL, BASE_WATER_VOLUME_BELOW_5, 0, 0 };

			double levelBeforeFloorAddedForWaterHigherCell = cellWithWaterHigherThanFloorAdded.getLevel();

			double waterVolumeReplacedInWaterLessCell = waterLessCell.addFloor(FLOOR_LEVEL_TO_ADD_ABOVE_5);
			double waterVolumeReplacedInWaterHigherCell = cellWithWaterHigherThanFloorAdded.addFloor(FLOOR_LEVEL_TO_ADD_BELOW_5);
			double waterVolumeReplacedInWaterLowerCell = cellWithWaterLowerThanFloorAdded.addFloor(FLOOR_LEVEL_TO_ADD_ABOVE_5);

			// For water less cell: check that level is correct, that is has no water and that volume replaced is 0
			Assert::AreEqual(waterLessCell.getLevel(), static_cast<double>(BASE_FLOOR_LEVEL + FLOOR_LEVEL_TO_ADD_ABOVE_5));
			Assert::IsFalse(waterLessCell.hasWater());
			Assert::AreEqual(waterVolumeReplacedInWaterLessCell, 0.0);

			// For cell with water higher than floor added: check that level hasn't changed, that is still has water, and that volume replaced is floor volume
			Assert::AreEqual(cellWithWaterHigherThanFloorAdded.getLevel(), levelBeforeFloorAddedForWaterHigherCell);
			Assert::IsTrue(cellWithWaterHigherThanFloorAdded.hasWater());
			Assert::AreEqual(waterVolumeReplacedInWaterHigherCell, static_cast<double>(FLOOR_LEVEL_TO_ADD_BELOW_5));

			// For cell with water lower than floor added: check that level is now floor level, that is now have no more water, and that volume replaced is whole water volume
			Assert::AreEqual(cellWithWaterLowerThanFloorAdded.getLevel(), static_cast<double>(BASE_FLOOR_LEVEL + FLOOR_LEVEL_TO_ADD_ABOVE_5));
			Assert::IsFalse(cellWithWaterLowerThanFloorAdded.hasWater());
			Assert::AreEqual(waterVolumeReplacedInWaterLowerCell, BASE_WATER_VOLUME_BELOW_5);
		}
	};
}
