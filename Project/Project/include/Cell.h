#pragma once
#include "globals.h"
#include <vector>
#include <raylib.h>
#include <raymath.h>

class Cell
{
public:
	Cell(CellType t_type = CellType::EMPTY);
	void addArc(Cell* t_cell);
	void visit() { m_visited = true; }
	void unvisit() { m_visited = false; }
	bool visited() { return m_visited; }
	void visitAllGoalNeighbours();

	void mark() { m_marked = true; }
	void unmark() { m_marked = false; }
	bool marked() { return m_marked; }

	void setValue(int t_x, int t_y) { m_x = t_x; m_y = t_y; }
	int getX() { return m_x; }
	int getY() { return m_y; }

	void setAStarValues(Vector2 t_goalPos);

	CellType getType() { return m_type; }
	void setType(CellType t_type) { m_type = t_type; }

	std::vector<Cell*>& getArcList() { return m_arcs; }

	//A* values
	float m_heuristic;
	float m_g;
	float m_cost;
	float m_estimatedCost;
	Cell* m_previous;
private:
	CellType m_type;
	std::vector<Cell*> m_arcs;
	bool m_visited;
	bool m_marked;
	int m_x;
	int m_y;
};

