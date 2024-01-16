#pragma once

class CellPosition 
{
public:
	// Constructor
	CellPosition() = delete;
	CellPosition(int row, int column) noexcept;

	// Getters
	int getRow() const noexcept;
	int getColumn() const noexcept;

	// Operators
	friend bool operator==(const CellPosition&, const CellPosition&);
	friend bool operator!=(const CellPosition&, const CellPosition&);

	// Hasher
	struct CellPositionHasher {
		size_t operator()(const CellPosition& pos) const;
	};

private:
	int m_row;
	int m_column;
};