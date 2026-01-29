#include "Goal.h"
#include <iostream>

Goal::Goal(Color t_c, float t_x, float t_y) : Wall(t_c, t_x, t_y)
{

}

void Goal::collision(int t_damage, Vector2 t_pos)
{
	if (m_alive)
	{
		LevelLoader::addProgress();
		m_alive = false;
	}
}
