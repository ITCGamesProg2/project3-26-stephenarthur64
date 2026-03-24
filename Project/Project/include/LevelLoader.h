#pragma once
#include <vector>
#include <fstream>
#include <json.hpp>
#include "Wall.h"
#include "EnemyLight.h"
#include "EnemyHeavy.h"
#include "EnemySupport.h"
#include "Goal.h"
#include "Door.h"
#include "Player.h"

static class LevelLoader
{
public:
	static void LoadLevel(std::vector<Wall>& t_w, std::vector<Goal>& t_g, std::vector<NPC>& t_e, std::vector<EnemySupport>& t_es, std::vector<Door>& t_d, TimeAbilities& t_bossType, Vector2& t_bossPos);
	static void addProgress();
	static void clearProgress();
	static bool isNextLevelReady();
	static bool isAtEnd();
	static void saveFile(int t_file);
	static void loadFile(int t_file);
	static void clearFile(int t_file);
	static void setPlayerRef(Player* t_p);
};

