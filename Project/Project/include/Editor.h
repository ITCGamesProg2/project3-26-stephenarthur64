#pragma once
#include "raylib.h"
#include <vector>
#include <fstream>
#include <string>
#include <json.hpp>
#include "Wall.h"
#include "NPC.h"
#include "Door.h"
#include "Goal.h"
#include "GameObject.h"
#include "globals.h"

static class Editor
{
public:
	static void handleInputs(bool& t_placing, Camera2D& t_cam);
	static void cycleStates();
	static void changeRoom(int t_direction);
	static int getRoom();
	static std::string getState();
	static void calculateMouse(Vector2 t_mouse);
	static void placeObject();
	static void placeWall();
	static void placeDoor();
	static void placeGoal();
	static void placeLight();
	static void placeHeavy();
	static void placeSupport();
	static void setWallReference(std::vector<Wall>* t_w);
	static void setNPCReference(std::vector<NPC>* t_e);
	static void setSupportReference(std::vector<EnemySupport>* t_es);
	static void setDoorReference(std::vector<Door>* t_d);
	static void setGoalReference(std::vector<Goal>* t_g);
	static void drawPlacing();
};

