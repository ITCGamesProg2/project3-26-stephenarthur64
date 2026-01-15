#include "Player.h"

Player::Player(Color t_c, float t_r) : GameObject(t_c, t_r), MAX_SPEED(6.0f), MIN_SPEED(0.01f), m_light(m_position)
{
	m_speed = 0.4f;
	Vector2 end = { m_position.x + 1000, m_position.y };
	m_light.setEnd(end);
}

void Player::update()
{
	if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
	{
		move();
	}
	m_light.setStart(m_position);
	m_light.process();
}

void Player::move()
{
	m_position += m_velocity;
	m_velocity *= 0.94f;
}

void Player::draw()
{
	GameObject::draw();

	m_light.draw();
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
		m_light.execute();
		break;
	case HEAVY:
		break;
	default:
		break;
	}
}
