#pragma once
#include "globals.h"
#include <vector>

class Cell
{
public:
	Cell(CellType t_type = CellType::EMPTY);
	void addArc(Cell* t_cell);

	CellType getType() { return m_type; }

private:
	CellType m_type;
	std::vector<Cell*> m_arcs;
};

