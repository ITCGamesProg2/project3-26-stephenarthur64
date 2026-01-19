#include "Player.h"
#include <iostream>

Player::Player(Color t_c, float t_r) : GameObject(t_c, t_r), MAX_SPEED(6.0f), MIN_SPEED(0.01f), m_light(m_position), m_heavy(m_position), m_special(m_position), 
										DEFAULT_FRICTION(0.94f), m_comboCurrent(0), COMBO_MAX(3), COMBO_MAX_COOLDOWN(10.5f), m_comboCountdown(0.0f)
{
	m_speed = 0.4f;
	m_friction = DEFAULT_FRICTION;
	CollisionCheck::setAttackReference(LIGHT, &m_light);
	CollisionCheck::setAttackReference(HEAVY, &m_heavy);
	CollisionCheck::setAttackReference(SPECIAL, &m_special);
	CollisionCheck::setPlayerReference(this);

	m_combo.push_back(&m_light);
	m_combo.push_back(&m_light);
	m_combo.push_back(&m_heavy);
	m_combo.push_back(&m_special);
}

void Player::update()
{
	if (m_comboCountdown < COMBO_MAX_COOLDOWN)
	{
		m_comboCountdown += GetFrameTime();
	}

	if (m_comboCurrent == COMBO_MAX)
	{
		m_combo.back()->execute();
		m_comboCurrent = 0;
		std::cout << m_comboCurrent << "\n";
	}

	if (!m_special.isRunning())
	{
		m_friction = DEFAULT_FRICTION;
	}

	move();

	m_light.setStart(m_position);
	m_light.process();

	m_heavy.setStart(m_position);
	m_heavy.process();

	m_special.setStart(m_position);
	m_special.process();
}

void Player::move()
{
	m_position += m_velocity;
	m_velocity *= m_friction;
}

void Player::draw()
{
	GameObject::draw();

	m_light.draw();
	m_heavy.draw();
	m_special.draw();
}

void Player::collision(bool t_damage, Vector2 t_pos)
{
}

void Player::addForce(Vector2 t_direction)
{
	m_velocity += t_direction * m_speed;
	if (m_velocity.x > MAX_SPEED)
	{
		m_velocity.x = MAX_SPEED;
	}
	if (m_velocity.y > MAX_SPEED)
	{
		m_velocity.y = MAX_SPEED;
	}
}

bool Player::rewind()
{
	m_newTime = Timeline::rewind();

	if (m_newTime.position.x != 0.0f)
	{
		m_velocity = m_newTime.velocity;
		m_position = m_newTime.position;
		return true;
	}
	return false;
}

Time Player::generateTime()
{
	m_newTime.position = m_position;
	m_newTime.radius = m_radius;
	m_newTime.velocity = m_velocity;

	return m_newTime;
}

void Player::useAttack(AttackTypes t_attack)
{
	switch (t_attack)
	{
	case LIGHT:
		if (m_light.canAttack())
		{
			comboCheck(t_attack);
		}
		m_light.execute();
		break;
	case HEAVY:
		if (m_heavy.canAttack())
		{
			comboCheck(t_attack);
		}
		m_heavy.execute();
		break;
	case SPECIAL:
		m_comboCurrent = 0;
		m_special.execute();
		m_friction = 0.99f;
		break;
	default:
		break;
	}
}

void Player::comboCheck(AttackTypes t_attack)
{
	if (m_comboCurrent < m_combo.size() - 1 && m_comboCountdown < COMBO_MAX_COOLDOWN)
	{
		if (m_combo.at(m_comboCurrent)->getType() == t_attack)
		{
			m_comboCurrent++;
			m_comboCountdown = 0.0f;
			std::cout << m_comboCurrent << "\n";
		}
		else
		{
			m_comboCurrent = 0;
			std::cout << m_comboCurrent << "\n";
		}
	}
	else
	{
		m_comboCountdown = 0.0f;
		m_comboCurrent = 0;
	}
}

void Player::addToCombo(Attack* t_attack)
{
	m_combo.push_back(t_attack);
}
