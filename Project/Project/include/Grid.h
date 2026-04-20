#pragma once
#include "Cell.h"

class Grid
{
public:
    static void initGrid();
    static void setGridData(int t_x, int t_y, int t_sizeX, int t_sizeY, CellType t_type);
    static Cell* getGridData(int t_x, int t_y);
};

