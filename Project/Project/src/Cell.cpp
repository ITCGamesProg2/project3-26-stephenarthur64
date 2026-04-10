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

void Cell::visitAllGoalNeighbours()
{
	if (!m_visited)
	{
		m_visited = true;

		for (Cell* arc : m_arcs)
		{
			if (arc->getType() == CellType::GOAL)
			{
				arc->visitAllGoalNeighbours();
			}
		}
	}
}
