#pragma once
#include "globals.h"
#include <vector>

class Cell
{
public:
	Cell(CellType t_type = CellType::EMPTY);
	void addArc(Cell* t_cell);

	CellType getType() { return m_type; }
	void setType(CellType t_type) { m_type = t_type; }


private:
	CellType m_type;
	std::vector<Cell*> m_arcs;
};

