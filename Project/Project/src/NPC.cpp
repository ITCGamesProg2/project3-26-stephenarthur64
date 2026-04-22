#include "NPC.h"

NPC::NPC(Color t_c, float t_r) : GameObject(t_c, t_r), m_minDistance(40000), MAX_SPEED(0.4f), MIN_SPEED(0.1f), m_surprise(false), MAX_HEAL_TIMER(1.0f), m_healTimer(0.0f), 
									MAX_DIST(400000)
{
	m_speed = 0.3f;
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

	if (!m_surprise)
	{
		decideTarget();

		if (m_target == m_playerTarget && m_distToTarget < MAX_DIST)
		{
			addForce(m_speed);
		}
		else if (m_distToTarget < MAX_DIST && m_targetCrumb != nullptr)
		{
			if (m_distToTarget > 10000)
			{
				addForce(m_speed);
			}
			else
			{
				crumbFound(*m_targetCrumb);

				decideTarget();
				addForce(m_speed);
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
	if (checkValidPlayerTarget())
	{
		m_target = m_playerTarget;
		m_distToTarget = Vector2DistanceSqr(m_position, m_target);
		m_targetCrumb = nullptr;
	}
	else
	{
		if (m_breadcrumb != nullptr && m_breadcrumb->size() > 0)
		{
			if (!m_targetCrumb || std::find(m_foundCrumbs.begin(), m_foundCrumbs.end(), m_targetCrumb.get()->getPosition()) != m_foundCrumbs.end())
			{
				m_targetCrumb = std::make_shared<Crumb>(m_breadcrumb->front());
				m_target = m_targetCrumb.get()->getPosition();
			}

			m_distToTarget = Vector2DistanceSqr(m_position, m_target);

			for (Crumb& c : *m_breadcrumb)
			{
				if (std::find(m_foundCrumbs.begin(), m_foundCrumbs.end(), c.getPosition()) == m_foundCrumbs.end())
				{
					if (Vector2DistanceSqr(m_position, c.getPosition()) < m_distToTarget && Vector2DistanceSqr(m_position, c.getPosition()) > 1000)
					{
						m_targetCrumb = std::make_shared<Crumb>(c);
						m_target = m_targetCrumb.get()->getPosition();
						m_distToTarget = Vector2DistanceSqr(m_position, m_target);
					}
				}
			}
		}
	}
}

bool NPC::checkValidPlayerTarget()
{
	bool finish = false;
	Vector2 towardsPlayer = m_playerTarget - m_position;
	towardsPlayer = Vector2Normalize(towardsPlayer);
	towardsPlayer *= 100;
	m_tempTarget = m_position;

	Vector2 playerCell = { m_playerTarget.x / 100, m_playerTarget.y / 100 };

	while (!finish)
	{
		m_tempTarget += towardsPlayer;

		if (Grid::getGridData(m_tempTarget.x / 100, m_tempTarget.y / 100)->getType() == CellType::WALL)
		{
			finish = true;
			return false;
		}

		if ((int)(m_tempTarget.x / 100) == (int)playerCell.x && (int)(m_tempTarget.y / 100) == (int)playerCell.y)
		{
			finish = true;
		}
	}

	return true;
}

void NPC::crumbFound(Crumb& t_c)
{
	if (m_foundCrumbs.size() >= 10)
	{
		m_foundCrumbs.pop_front();
	}
	m_foundCrumbs.push_back(t_c.getPosition());
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
