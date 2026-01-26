#pragma once
#include "raylib.h"
#include "GameObject.h"
#include "Attack.h"
#include "Wall.h"
#include "globals.h"

static class CollisionCheck
{
public:
	static void CheckCollisionsGameObject(GameObject& t_go1, GameObject& t_go2);
	static void CheckCollisionsWall(GameObject& t_go, Wall& t_wall);
	static void CheckCollisionAttack(Attack* t_attack, GameObject& t_go);
	static void setPlayerReference(GameObject *t_player);
};

