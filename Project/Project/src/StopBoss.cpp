#include "StopBoss.h"

StopBoss::StopBoss()
{
	m_speed = 0.3f;
	m_attackType = HEAVY;
	m_attack = new SpecialAttack({ -1000, -1000 });
	m_attack->setWindupScale(0.1f);
	m_attack->setCooldownScale(0.5f);
	m_maxHealth = 7;
	m_health = m_maxHealth;
	m_upgrade.setAbility(TimeAbilities::MAX);

	m_target = { -1000.0f, 0.0f };
}

void StopBoss::draw()
{
	if (m_alive)
	{
		NPC::draw();
	}
}

void StopBoss::update()
{
	if (!m_alive)
	{
		m_attack->setStart({ -1000.0f, 0.0f });
		m_position = { -1000.0f, 0.0f };
		return;
	}

	GameObject::update();

	if (Vector2DistanceSqr(m_target, m_position) > m_minDistance && m_attack->isRunning())
	{
		addForce(m_speed);
	}
	
	if (m_attack->canAttack())
	{
		m_attack->execute(m_target);
	}

	m_attack->setStart(m_position);
	m_attack->process();

	if (m_attack->isRunning())
	{
		move();
	}

	if (m_attack->isCollided())
	{
		m_target = m_velocity * 10000;
		std::cout << "bounced\n";
	}
}

void StopBoss::immuneCheck(Vector2 t_playerPos)
{
	if (m_attack->isRunning())
	{
		m_canDamage = false;
	}
	else
	{
		m_canDamage = true;
	}
}
