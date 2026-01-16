#include "NPC.h"

NPC::NPC(Color t_c, float t_r) : GameObject(t_c, t_r), MIN_DISTANCE(10000)
{
	m_speed = 2.0f;
}

void NPC::update()
{
	move();
}

void NPC::move()
{
	//if (Vector2DistanceSqr(m_target, m_position) > MIN_DISTANCE)
	//{
	//	m_velocity = m_target - m_position;
	//	m_velocity = Vector2Normalize(m_velocity);
	//	m_velocity *= m_speed;
	//}
	m_position += m_velocity;
	m_velocity *= 0.94f;
}

void NPC::collision(bool t_damage, Vector2 t_pos)
{
	Vector2 knockback = t_pos - m_position;

	knockback = Vector2Normalize(knockback);
	knockback *= -5.0f;
	m_velocity = knockback;
}
