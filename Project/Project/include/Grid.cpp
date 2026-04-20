#include "Grid.h"

static Cell m_grid[50][50];

void Grid::initGrid()
{
    for (int x = 0; x < 50; x++)
    {
        for (int y = 0; y < 50; y++)
        {
            m_grid[x][y].setValue(x, y);

            if (x - 1 >= 0)
            {
                m_grid[x][y].addArc(&m_grid[x - 1][y]);
            }
            if (x + 1 < 50)
            {
                m_grid[x][y].addArc(&m_grid[x + 1][y]);
            }
            if (y - 1 >= 0)
            {
                m_grid[x][y].addArc(&m_grid[x][y - 1]);
            }
            if (y + 1 < 50)
            {
                m_grid[x][y].addArc(&m_grid[x][y + 1]);
            }
        }
    }
}

void Grid::setGridData(int t_x, int t_y, int t_sizeX, int t_sizeY, CellType t_type)
{
    t_x /= 100;
    t_y /= 100;
    t_sizeX /= 100;
    t_sizeY /= 100;

    for (int x = t_x; x < t_x + t_sizeX; x++)
    {
        for (int y = t_y; y < t_y + t_sizeY; y++)
        {
            m_grid[x][y].setType(t_type);
        }
    }
}

Cell* Grid::getGridData(int t_x, int t_y)
{
    return &m_grid[t_x][t_y];
}
