#pragma once

#include "pch.h"
#include "CellIdentifier.hpp"

class Cell
{
public:
	// Constructors
	Cell() = delete;
	Cell(int floorLevel, double waterLevel, int row, int column) noexcept;
	
	// Getters
	const CellIdentifier& getCellIdentifier() const noexcept;
	bool hasWater() const noexcept;
	double getLevel() const noexcept;
	double getWaterLevel() const noexcept;
	int getFloorLevel() const noexcept;


	// Methods
	// Returns the volume of water that has been replaced by the floor
	double addFloor(int floorHeight) noexcept;
	void addWater(double waterVolume) noexcept;

	// Operators
	friend std::ostream& operator<<(std::ostream& os, const Cell& cell);
	friend bool operator==(const Cell& cell1, const Cell& cell2);
	friend bool operator!=(const Cell& cell1, const Cell& cell2);

private:
	CellIdentifier m_cellIdentifier;
	int m_floorLevel;
	double m_waterLevel;

	double waterVolume() const noexcept;
};
