#include "RewindBoss.h"

RewindBoss::RewindBoss() : Boss()
{
	m_attackType = HEAVY;
	m_attack = new HeavyAttack({ -1000, -1000 });
	m_attack->setCooldownScale(1.0f);
	m_attack->setWindupScale(1.0f);
	m_maxHealth = 10;
	m_health = m_maxHealth;
}

void RewindBoss::draw()
{
	if (m_alive)
	{
		NPC::draw();

		DrawLine(m_position.x, m_position.y, m_target.x, m_target.y, YELLOW);
	}
}

void RewindBoss::immuneCheck(Vector2 t_playerPos)
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
