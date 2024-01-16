#include "Command.hpp"

std::istream& operator>>(std::istream& in, Command& command)
{
	std::string commandString;
	in >> commandString;

	if (commandString == "new")
	{
		command.m_commandType = CommandType::NewWaterGridSimulator;
	}
	else if (commandString == "addWater")
	{
		command.m_commandType = CommandType::AddWater;
		in >> command.m_row >> command.m_column >> command.m_volume;
	}
	else if (commandString == "addFloor")
	{
		command.m_commandType = CommandType::AddFloor;
		in >> command.m_row >> command.m_column >> command.m_level;
	}
	else if (commandString == "exit")
	{
		command.m_commandType = CommandType::Exit;
	}
	else
	{
		throw std::invalid_argument("Invalid command");
	}

	return in;
}

CommandType Command::getCommandType() const noexcept
{
	return m_commandType;
}

void Command::execute(WaterGridSimulator& simulator) const
{
	switch (m_commandType)
	{	
	case CommandType::NewWaterGridSimulator:
		break;
	case CommandType::AddWater:
		simulator.addWater(m_row, m_column, m_volume);
		break;
	case CommandType::AddFloor:
		simulator.addFloor(m_row, m_column, m_level);
		break;
	case CommandType::Exit:
		break;
	default:
		break;
	}
}
