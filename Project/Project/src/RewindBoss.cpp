#include "RewindBoss.h"

RewindBoss::RewindBoss() : Boss()
{
	m_attackType = HEAVY;
	m_attack = new HeavyAttack({ -1000, -1000 });
	m_attack->setCooldownScale(1.0f);
	m_attack->setWindupScale(1.0f);
	m_maxHealth = 1;
	m_health = m_maxHealth;
	m_upgrade.setAbility(TimeAbilities::SKIP);
}

void RewindBoss::draw()
{
	if (m_alive)
	{
		NPC::draw();

		if (!m_canDamage)
		{
			m_shieldPos = Vector2MoveTowards(m_position, m_target, m_radius);

			m_rotateOriginLine = { m_shieldPos.x, m_shieldPos.y * 10 };

			m_normLineToShield = Vector2Normalize(Vector2Subtract(m_rotateOriginLine, m_shieldPos));
			m_normTargetToShield = Vector2Normalize(Vector2Subtract(m_target, m_shieldPos));

			m_shieldAngle = Vector2Angle(m_normLineToShield, m_normTargetToShield) * RAD2DEG;
		}

		
		DrawTexturePro(*m_sprite, { 320, 0, 64, 64 }, { m_shieldPos.x, m_shieldPos.y, 100 * 1.5f, 100 * 1.5f }, { 64, 64}, m_shieldAngle, WHITE);
	}
	else
	{
		m_upgrade.draw();
	}
}
