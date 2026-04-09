#include "Cell.h"

Cell::Cell(CellType t_type) : m_type(t_type)
{
}

void Cell::addArc(Cell* t_cell)
{
	if (m_arcs.size() < 4)
	{
		m_arcs.push_back(t_cell);
	}
}
