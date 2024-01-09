#pragma once

#include "pch.h"

class Cell
{
public:
	// Constructors
	Cell() = delete;
	Cell(int floorLevel, double waterLevel) noexcept;
	
	// Getters and Setters
	bool hasWater() const noexcept;
	double getLevel() const noexcept;
	double getWaterLevel() const noexcept;
	void setWaterLevel(double waterLevel) noexcept;
	int getFloorLevel() const noexcept;
	void setFloorLevel(int floorLevel) noexcept;

	// Operators
	friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

private:
	int m_floorLevel;
	double m_waterLevel;
};
