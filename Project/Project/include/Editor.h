#pragma once
#include "raylib.h"
#include <vector>
#include <fstream>
#include <string>
#include <json.hpp>
#include "Wall.h"
#include "GameObject.h"
#include "globals.h"

static class Editor
{
public:
	static void ChangeState(EditState t_choice);
	static EditState getState();
	static void calculateMouse(Vector2 t_mouse);
	static void placeObject(std::vector<GameObject>& t_go);
	static void placeWall(std::vector<GameObject>& t_w);

private:
	static EditState m_state;
	static Vector2 m_placePos;
	static Vector2 m_placeSize;
	static Vector2 m_mousePos;
};

