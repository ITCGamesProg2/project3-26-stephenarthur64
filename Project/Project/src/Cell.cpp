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

void Cell::setAStarValues(Vector2 t_goalPos)
{
	m_g = INFINITY;
	m_cost = 1.0f;

	m_heuristic = Vector2Distance({ (float)m_x, (float)m_y }, t_goalPos);

	m_estimatedCost = -1;

	m_previous = nullptr;
}
