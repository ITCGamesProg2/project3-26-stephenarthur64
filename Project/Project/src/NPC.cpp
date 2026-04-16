#include "NPC.h"

NPC::NPC(Color t_c, float t_r) : GameObject(t_c, t_r), m_minDistance(40000), MAX_SPEED(0.4f), MIN_SPEED(0.1f), m_surprise(false), MAX_HEAL_TIMER(1.0f), m_healTimer(0.0f)
{
	m_speed = 0.3f;
	m_health = 3;
	m_maxIFrames = 0.5f;
	m_alive = true;
}

NPC::~NPC()
{
	//delete m_attack;
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

	if (m_targetCrumb == nullptr)
	{
		decideTarget();
	}

	if (m_distToTarget < 200000)
	{
		if (m_target == m_playerTarget)
		{
			if (m_distToTarget > m_minDistance)
			{
				addForce(m_speed);
			}
		}
		else
		{
			addForce(m_speed);
			if (m_distToTarget <= m_minDistance)
			{
				if (m_targetCrumb != nullptr)
				{
					std::cout << "next\n";
					if (m_targetCrumb->m_next != nullptr)
					{
						m_targetCrumb = m_targetCrumb->m_next;
						m_target = m_targetCrumb->m_position;
						m_distToTarget = Vector2DistanceSqr(m_targetCrumb->m_position, m_position);
					}
				}
			}
		}

		if (Vector2DistanceSqr(m_position, m_playerTarget) < m_minDistance)
		{
			if (m_attack->canAttack())
			{
				m_attack->execute(m_target);
			}
		}

		move();
	}

	m_attack->setStart(m_position);
	m_attack->process();
}

void NPC::move()
{
	m_position += m_velocity;
	m_velocity *= 0.94f;
}

void NPC::draw()
{
	if (m_selected)
	{
		DrawCircle(m_position.x, m_position.y, m_radius + 5.0f, GREEN);
	}

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

void NPC::decideTarget()
{
	if (m_breadcrumb != nullptr && m_breadcrumb->size() > 0)
	{
		m_distToTarget = Vector2DistanceSqr(m_breadcrumb->front().m_position, m_position);
		m_target = m_breadcrumb->front().m_position;
		m_targetCrumb = &m_breadcrumb->front();

		for (Crumb& b : *m_breadcrumb)
		{
			if (Vector2DistanceSqr(b.m_position, m_position) < m_distToTarget && Vector2DistanceSqr(b.m_position, m_position) > 1000)
			{
				m_distToTarget = Vector2DistanceSqr(b.m_position, m_position);
				m_target = b.m_position;
				m_targetCrumb = &b;
			}
		}
	}
}

void NPC::heal()
{
	if (m_healTimer >= MAX_HEAL_TIMER)
	{
		if (m_health + 1 < m_maxHealth)
		{
			m_health++;
		}
		m_healTimer = 0.0f;
	}
	else
	{
		m_healTimer += GetFrameTime();
	}
}
