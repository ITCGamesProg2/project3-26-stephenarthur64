#include "Goal.h"
#include <iostream>

Goal::Goal(Color t_c, float t_x, float t_y) : GameObject(t_c, t_x), m_sizeX(t_x), m_sizeY(t_y)
{
	m_spriteScale = 1.0f;
}

void Goal::collision(int t_damage, Vector2 t_pos)
{
	if (m_alive)
	{
		LevelLoader::addProgress();
		m_alive = false;
	}
}

void Goal::draw()
{
	DrawTexturePro(*m_sprite, m_animation.getFrame(), { m_position.x, m_position.y, m_sizeX, m_sizeY }, { 0.0f, 0.0f }, 0, WHITE);
}
