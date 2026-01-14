#include "Player.h"

Player::Player(Color t_c, float t_r) : GameObject(t_c, t_r)
{
	m_speed = 3.0f;
}

void Player::update()
{
	move();
}

void Player::move()
{
	m_position += m_velocity;
}

void Player::moveDirection(Vector2 t_direction)
{
	m_velocity = t_direction * m_speed;
}

void Player::rewind(Time t_new)
{
	m_velocity = { 0.0f,  0.0f };
	m_position = t_new.position;
}
