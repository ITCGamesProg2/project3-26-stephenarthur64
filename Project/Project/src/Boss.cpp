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

		DrawLine(m_position.x, m_position.y, m_target.x, m_target.y, YELLOW);
	}
}

void Boss::update()
{
	NPC::update();
}

void Boss::damage(int t_amount)
{
	if (m_canDamage)
	{
		GameObject::damage(t_amount);
	}
}
