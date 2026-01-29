#pragma once
#include "Wall.h"

class Goal :
	public Wall
{
public:
	Goal(Color t_c, float t_x, float t_y);
	virtual void collision(int t_damage, Vector2 t_pos) override;
};

#include "LevelLoader.h"
