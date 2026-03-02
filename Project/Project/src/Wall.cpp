#include "Wall.h"
#include <iostream>

Wall::Wall(Color t_c, float t_x, float t_y) : GameObject(t_c, t_x), m_sizeX(t_x), m_sizeY(t_y)
{
	m_colour = t_c;
}

void Wall::move()
{
}

void Wall::collision(int t_damage, Vector2 t_pos)
{
}

void Wall::draw()
{
	DrawRectangle(m_position.x, m_position.y, m_sizeX, m_sizeY, m_colour);
	DrawTexturePro(AssetManager::getWallSprite(), { 0, 0, 32 * (m_sizeX / 100), 32 * (m_sizeY / 100)}, {m_position.x, m_position.y, m_sizeX, m_sizeY}, {0, 0}, 0.0f, WHITE);
}
