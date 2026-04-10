#pragma once
#include "globals.h"
#include <vector>

class Cell
{
public:
	Cell(CellType t_type = CellType::EMPTY);
	void addArc(Cell* t_cell);
	void visit() { m_visited = true; }
	void unvisit() { m_visited = false; }
	bool visited() { return m_visited; }
	void visitAllGoalNeighbours();

	void setValue(int t_x, int t_y) { m_x = t_x; m_y = t_y; }
	int getX() { return m_x; }
	int getY() { return m_y; }

	CellType getType() { return m_type; }
	void setType(CellType t_type) { m_type = t_type; }

	std::vector<Cell*>& getArcList() { return m_arcs; }

private:
	CellType m_type;
	std::vector<Cell*> m_arcs;
	bool m_visited;
	int m_x;
	int m_y;
};

