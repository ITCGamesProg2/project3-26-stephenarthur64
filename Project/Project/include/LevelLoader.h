#pragma once
#include <vector>
#include "Wall.h"
#include "NPC.h"

static class LevelLoader
{
	static void LoadLevels(std::vector<Wall>& t_walls, std::vector<NPC>& t_enemies);
	static void LoadCombos();
};

