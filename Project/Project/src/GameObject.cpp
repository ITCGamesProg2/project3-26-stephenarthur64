#include "GameObject.h"

GameObject::GameObject(Color t_c, float t_r) : m_colour(t_c), m_radius(t_r), m_position({ 100.0f, 100.0f }), m_velocity({0.0f, 0.0f}), m_alive(true)
{
}

void GameObject::update()
{
	if (m_invincibilityFrames > 0.0f)
	{
		m_invincibilityFrames -= GetFrameTime();
	}
}

void GameObject::draw()
{
	if (m_alive)
	{
		if (m_invincibilityFrames > 0.0f)
		{
			DrawCircle(m_position.x, m_position.y, m_radius, ORANGE);
		}
		else
		{
			DrawCircle(m_position.x, m_position.y, m_radius, m_colour);
		}
	}
}

void GameObject::applyKnockback(Vector2 t_collidePos, float t_force)
{
	Vector2 collide;

	if (m_position.x > t_collidePos.x + 100)
	{
		collide.x = m_position.x - m_radius;
	}
	else
	{
		collide.x = t_collidePos.x;
	}
	if (m_position.y > t_collidePos.y + 100)
	{
		if (collide.x == m_position.x - m_radius)
		{
			collide.y = m_position.y - m_radius;
		}
		else
		{
			collide.y = m_position.y;
		}
	}
	else
	{
		collide.y = t_collidePos.y;
	}

	Vector2 knockback;
	knockback = m_position - collide;
	knockback = Vector2Normalize(knockback);

	m_velocity += knockback * t_force;
}

void GameObject::damage(int t_amount)
{
	if (m_invincibilityFrames <= 0.0f)
	{
		m_health -= t_amount;

		if (m_health <= 0)
		{
			m_alive = false;
		}
		else
		{
			m_invincibilityFrames = m_maxIFrames;
		}
	}
}

void GameObject::respawn()
{
	m_health = m_maxHealth;
	m_alive = true;
	m_velocity = { 0.0f, 0.0f };
}
