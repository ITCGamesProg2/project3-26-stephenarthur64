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
#include "Boss.h"
#include "globals.h"
#include "Button.h"

class Editor
{
public:
	Editor();
	void setSpawn(Vector2 t_spawn) { m_spawnPos = t_spawn; }
	Vector2 getSpawn() { return m_spawnPos; }
	void drawUI();
	void handleInputs(bool& t_placing, Camera2D& t_cam);
	void checkDoorEnemyClick(Vector2 t_mouse);
	std::string getState();
	void calculateMouse(Vector2 t_mouse);
	void placeObject();
	void placeWall();
	void placeDoor();
	void placeGoal();
	void placeLight();
	void placeHeavy();
	void placeSupport();
	void writeObjectsToFile();
	void setWallReference(std::vector<Wall>* t_w);
	void setNPCReference(std::vector<NPC>* t_e);
	void setSupportReference(std::vector<EnemySupport>* t_es);
	void setDoorReference(std::vector<Door>* t_d);
	void setGoalReference(std::vector<Goal>* t_g);
	void drawPlacing();
	bool checkPlacing(Vector2 t_pos, float t_radius);
	bool checkPlacing(Vector2 t_pos, float t_x, float t_y);
	void saveFile();
	bool resumeTriggered() { return m_resume.triggered(); }
	void resetResume() { m_resume.resetTrigger(); }
	void undo();

private:
	int m_entityCount = 0;
	int m_room = 1;
	EditState m_state;
	Vector2 m_placePos = { INFINITY, 1.0f };
	Vector2 m_placeSize;
	Vector2 m_mousePos;
	std::vector<Wall>* m_walls;
	std::vector<NPC>* m_enemies;
	std::vector<EnemySupport>* m_es;
	std::vector<Door>* m_doors;
	std::vector<Goal>* m_goals;
	int m_currentState = 0;
	EditState m_allStates[END] = { EditState::WALL, EditState::LIGHTENEMY, EditState::HEAVYENEMY, EditState::SUPPORTENEMY, EditState::GOAL, EditState::DOOR };
	std::string m_currentLevel = "levels/levelclear.json";
	nlohmann::json data;
	Button m_save;
	Button m_objectButtons[EditState::END];
	Button m_resume;
	Button m_undo;
	bool m_uiInteract;
	NPC* m_selectedEnemy;
	std::vector<EditState> m_actionList;
	Vector2 m_spawnPos;
};

