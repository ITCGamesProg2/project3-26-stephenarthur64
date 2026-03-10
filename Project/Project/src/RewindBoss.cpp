#include "RewindBoss.h"

RewindBoss::RewindBoss() : Boss()
{
	m_attackType = HEAVY;
	m_attack = new HeavyAttack({ -1000, -1000 });
	m_attack->setCooldownScale(1.0f);
	m_attack->setWindupScale(1.0f);
	m_maxHealth = 1;
	m_health = m_maxHealth;
	m_upgrade.setAbility(TimeAbilities::STOP);
}

void RewindBoss::draw()
{
	Boss::draw();

	if (m_alive)
	{
		DrawLine(m_position.x, m_position.y, m_target.x, m_target.y, YELLOW);
	}
}
