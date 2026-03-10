#include "Boss.h"

Boss::Boss() : NPC(RED, 80.0F)
{
}

void Boss::move()
{
}

void Boss::draw()
{
	if (m_alive)
	{
		NPC::draw();
	}
	else
	{
		m_upgrade.draw();
	}
}

void Boss::update()
{
	NPC::update();
	if (m_alive)
	{
		m_upgrade.setPosition(m_position);
	}
}

void Boss::damage(int t_amount)
{
	if (m_canDamage)
	{
		GameObject::damage(t_amount);
	}
}

void Boss::immuneCheck(Vector2 t_playerPos)
{
	if (m_target == t_playerPos)
	{
		m_canDamage = false;
	}
	else
	{
		m_canDamage = true;
	}
}
