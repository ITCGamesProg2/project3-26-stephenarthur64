#include "Door.h"

Door::Door(Color t_c, float t_x, float t_y) : Wall(t_c, t_x, t_y), m_enemyCount(0), m_enemyMax(0)
{
}

void Door::draw()
{
	if (m_alive)
	{
		DrawRectangle(m_position.x, m_position.y, m_sizeX, m_sizeY, m_colour);
	}
}

void Door::update()
{
	m_enemyCount = 0;

	for (NPC* e : m_enemies)
	{
		if (!e->isAlive())
		{
			m_enemyCount++;
		}
	}

	if (m_enemyCount >= m_enemyMax)
	{
		open();
	}
}

void Door::open()
{
	m_alive = false;
}

void Door::addEnemy(NPC* t_e)
{
	m_enemies.push_back(t_e);
	m_enemyMax++;
}