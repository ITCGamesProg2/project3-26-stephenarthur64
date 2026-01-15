#include "Player.h"

Player::Player(Color t_c, float t_r) : GameObject(t_c, t_r), MAX_SPEED(6.0f), MIN_SPEED(0.01f)
{
	m_speed = 0.4f;
}

void Player::update()
{
	if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
	{
		move();
	}
}

void Player::move()
{
	m_position += m_velocity;
	m_velocity *= 0.94f;
}

void Player::moveDirection(Vector2 t_direction)
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

void Player::rewind(Time t_new)
{
	m_velocity = { 0.0f,  0.0f };
	m_position = t_new.position;
}
