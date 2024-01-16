#pragma once
#include <istream>
#include "../WaterGridSimulator/WaterGridSimulator.hpp"

enum class CommandType
{
	NewWaterGridSimulator,
	AddWater,
	AddFloor,
	Exit
};

class Command
{
public:
	friend std::istream& operator>>(std::istream& in, Command& command);
	void execute(WaterGridSimulator& simulator) const;
	CommandType getCommandType() const noexcept;

private:
	CommandType m_commandType;
	int m_row;
	int m_column;
	double m_volume;
	int m_level;
};

