#include "NPC.h"

NPC::NPC(Color t_c, float t_r) : GameObject(t_c, t_r), MIN_DISTANCE(12000), MAX_SPEED(0.4f), MIN_SPEED(0.1f)
{
	m_speed = 0.3f;
	m_health = 3;
	m_maxIFrames = 0.5f;
	m_alive = true;
}

void NPC::update()
{
	if (!m_alive)
	{
		m_attack->setStart({ -1000.0f, 0.0f });
		m_position = { -1000.0f, 0.0f };
		return;
	}

	GameObject::update();

	if (Vector2DistanceSqr(m_target, m_position) > MIN_DISTANCE)
	{
		addForce(m_speed);
	}
	else
	{
		if (m_attack->canAttack())
		{
			m_attack->execute(m_target);
		}
	}

	m_attack->setStart(m_position);
	m_attack->process();

	move();
}

void NPC::move()
{
	m_position += m_velocity;
	m_velocity *= 0.94f;
}

void NPC::draw()
{
	GameObject::draw();

	m_attack->draw();
}

void NPC::collision(int t_damage, Vector2 t_pos)
{
	if (t_damage)
	{
		damage(t_damage);
	}

	applyKnockback(t_pos);
}

void NPC::addForce(float t_amount)
{
	Vector2 direction = m_target - m_position;
	direction = Vector2Normalize(direction) * t_amount;
	m_velocity += direction;
}
