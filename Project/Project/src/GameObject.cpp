#include "GameObject.h"

GameObject::GameObject(Color t_c, float t_r) : m_colour(t_c), m_radius(t_r), m_position({ 100.0f, 100.0f }), m_velocity({0.0f, 0.0f}), m_alive(true), m_invincibilityFrames(0.0f),
												m_animation(4, 32, 32, 0.2f)
{
}

void GameObject::update()
{
	updateInvincibilityFrames();

	m_animation.nextFrame(GetFrameTime());
}

void GameObject::updateInvincibilityFrames()
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
			DrawTexturePro(*m_sprite, m_animation.getFrame(), {m_position.x, m_position.y, 100 * m_spriteScale, 100 * m_spriteScale}, {100 * m_spriteScale / 2.0f, 100 * m_spriteScale / 2.0f}, 0, ORANGE);
		}
		else
		{
			DrawCircle(m_position.x, m_position.y, m_radius, m_colour);
			DrawTexturePro(*m_sprite, m_animation.getFrame(), { m_position.x, m_position.y, 100 * m_spriteScale, 100 * m_spriteScale }, { 100 * m_spriteScale / 2.0f, 100 * m_spriteScale / 2.0f }, 0, WHITE);
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
	knockback = Vector2Scale(knockback, t_force);

	if (Vector2Length(m_velocity + knockback) < 30)
	{
		m_velocity += knockback;
	}
}

void GameObject::applyKnockbackWall(int t_x, int t_y)
{
	if (t_x != 0) // X
	{
		m_velocity.x = m_speed * t_x * 10;
	}
	else if (t_y != 0) // Y
	{
		m_velocity.y = m_speed * t_y * 10;
	}
}

Vector2 GameObject::nextPositionX()
{
	Vector2 value;
	if (m_velocity.x >= 0)
	{
		value.x = m_position.x + m_radius;
	}
	else
	{
		value.x = m_position.x - m_radius;
	}

	if (m_velocity.y >= 0)
	{
		value.y = m_position.y + m_radius;
	}
	else
	{
		value.y = m_position.y - m_radius;
	}
	value.x += m_velocity.x;
	return value;
}

Vector2 GameObject::nextPositionY()
{
	Vector2 value;
	if (m_velocity.x >= 0)
	{
		value.x = m_position.x + m_radius;
	}
	else
	{
		value.x = m_position.x - m_radius;
	}

	if (m_velocity.y >= 0)
	{
		value.y = m_position.y + m_radius;
	}
	else
	{
		value.y = m_position.y - m_radius;
	}
	value.y += m_velocity.y;
	return value;
}

Vector2 GameObject::nextPositionFull()
{
	Vector2 value;
	value = m_position;
	value += m_velocity;
	return value;
}

void GameObject::stopVelocity(int t_direction)
{
	if (t_direction == 0)
	{
		if (m_velocity.x != 0.0f)
		{
			if (!IsSoundPlaying(AssetManager::getSound("bump")))
			{
				PlaySound(AssetManager::getSound("bump"));
			}
		}
		m_velocity.x = 0.0f;
	}
	else if (t_direction == 1)
	{
		if (m_velocity.y != 0.0f)
		{
			if (!IsSoundPlaying(AssetManager::getSound("bump")))
			{
				PlaySound(AssetManager::getSound("bump"));
			}
		}
		m_velocity.y = 0.0f;
	}
	else
	{
		if (m_velocity.x != 0.0f && m_velocity.y != 0.0f)
		{
			if (!IsSoundPlaying(AssetManager::getSound("bump")))
			{
				PlaySound(AssetManager::getSound("bump"));
			}
		}
		m_velocity.x = 0.0f;
		m_velocity.y = 0.0f;
	}
}

void GameObject::damage(int t_amount)
{
	if (m_invincibilityFrames <= 0.0f)
	{
		m_health -= t_amount;
		PlaySound(AssetManager::getSound("hurt"));

		if (m_health <= 0)
		{
			PlaySound(AssetManager::getSound("die"));
			m_alive = false;
			m_position = { 0.0f, 0.0f };
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
