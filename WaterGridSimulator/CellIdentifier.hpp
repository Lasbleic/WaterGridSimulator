#pragma once

class CellIdentifier 
{
public:
	// Constructor
	CellIdentifier() = delete;
	CellIdentifier(int row, int column) noexcept;

	// Getters
	std::size_t getIdentifierHash() const noexcept;

	// Operators
	friend bool operator==(const CellIdentifier& cellIdentifier1, const CellIdentifier& cellIdentifier2);
	friend bool operator!=(const CellIdentifier& cellIdentifier1, const CellIdentifier& cellIdentifier2);

private:
	int m_row;
	int m_column;
};