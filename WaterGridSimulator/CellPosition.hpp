#pragma once

class CellPosition 
{
public:
	// Constructor
	CellPosition() = delete;
	CellPosition(int row, int column) noexcept;

	// Getters
	std::size_t getHash() const noexcept;

	// Operators
	friend bool operator==(const CellPosition&, const CellPosition&);
	friend bool operator!=(const CellPosition&, const CellPosition&);

private:
	int m_row;
	int m_column;
};