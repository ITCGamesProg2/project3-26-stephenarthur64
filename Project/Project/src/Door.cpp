#include "Door.h"

Door::Door(Color t_c, float t_x, float t_y) : Wall(t_c, t_x, t_y), m_enemyCount(0), m_enemyMax(0)
{
}

void Door::draw()
{
	if (m_alive)
	{
		DrawRectangle(m_position.x, m_position.y, m_sizeX, m_sizeY, m_colour);
		DrawTexturePro(AssetManager::getSprite("door"), {0, 0, 32 * (m_sizeX / 100), 32 * (m_sizeY / 100)}, {m_position.x, m_position.y, m_sizeX, m_sizeY}, {0, 0}, 0.0f, WHITE);
	}
}

void Door::drawDebug()
{
	for (NPC* e : m_enemies)
	{
		if (e->isAlive())
		{
			DrawLine(m_position.x, m_position.y, e->getPosition().x, e->getPosition().y, DARKGREEN);
		}
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
	if (m_alive)
	{
		PlaySound(AssetManager::getSound("door_open"));
	}
	m_alive = false;
}

void Door::addEnemy(NPC* t_e)
{
	m_enemies.push_back(t_e);
	m_enemyMax++;
}

