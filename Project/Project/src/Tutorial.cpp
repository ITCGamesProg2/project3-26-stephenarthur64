#include "Tutorial.h"

Tutorial::Tutorial(Color t_c, float t_x, float t_y) : Goal(t_c, t_x, t_y)
{
}

void Tutorial::update()
{
	if (m_alive)
	{
		Goal::update();
	}
	m_popup.update();
}

void Tutorial::collision(int t_damage, Vector2 t_pos)
{
	if (m_alive)
	{
		m_popup.spawn({ 600.0f, 200.0f }, { 50.0f, 50.0f });
		m_alive = false;
	}
}

void Tutorial::drawPopup()
{
	m_popup.draw();
}
