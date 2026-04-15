#include "SkipBoss.h"

SkipBoss::SkipBoss()
{
	m_speed = 0.8f;
	m_attackType = HEAVY;
	m_attack = new HeavyAttack({ -1000, -1000 });
	m_attack->setWindupScale(0.1f);
	m_attack->setCooldownScale(0.5f);
	m_maxHealth = 7;
	m_health = m_maxHealth;
	m_upgrade.setAbility(TimeAbilities::STOP);
}

void SkipBoss::draw()
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

void SkipBoss::immuneCheck(Vector2 t_playerPos)
{
	m_canDamage = true;
}
