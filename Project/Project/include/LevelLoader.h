#pragma once
#include <vector>
#include <fstream>
#include <json.hpp>
#include "Wall.h"
#include "EnemyLight.h"
#include "EnemyHeavy.h"
#include "Goal.h"
#include "Door.h"
#include "Player.h"

static class LevelLoader
{
public:
	static void LoadLevel(std::vector<Wall>& t_w, std::vector<Goal>& t_g, std::vector<EnemyLight>& t_l, std::vector<EnemyHeavy>& t_h, std::vector<Door>& t_d, Player& t_p);
	static void addProgress();
	static void clearProgress();
	static bool isNextLevelReady();
	static bool isAtEnd();
};

