#include "GameObject.h"

GameObject::GameObject(Color t_c, float t_r) : m_colour(t_c), m_radius(t_r), m_position({ 100.0f, 100.0f }), m_velocity({0.0f, 0.0f})
{
}

void GameObject::draw()
{
	DrawCircle(m_position.x, m_position.y, m_radius, m_colour);
}