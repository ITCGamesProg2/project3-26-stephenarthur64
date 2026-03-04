#include "Pickup.h"

Pickup::Pickup() : m_radius(25.0f), m_alive(true)
{
}

void Pickup::draw()
{
	if (m_alive)
	{
		DrawCircle(m_position.x, m_position.y, m_radius, YELLOW);
	}
}

void Pickup::setPosition(Vector2 t_pos)
{
	m_position = t_pos;
}

void Pickup::deactivate()
{
	m_alive = false;
	PlaySound(AssetManager::getSound("powerup"));
}
