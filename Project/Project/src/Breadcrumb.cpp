#include "Breadcrumb.h"

Breadcrumb::Breadcrumb() : MAX_POSITIONS(10), MAX_TIME(0.3f), m_timer(0.0f), m_canSpawn(false)
{
}

void Breadcrumb::drawCrumbs()
{
	for (Crumb& bc : m_crumbs)
	{
		bc.drawParticles();
	}
}

void Breadcrumb::spawn(Vector2 t_position)
{
	if (m_canSpawn)
	{
		if (m_crumbs.size() >= MAX_POSITIONS)
		{
			m_crumbs.pop_front();
		}

		if (m_crumbs.size() > 0)
		{
			Crumb* oldBack = &m_crumbs.back();

			m_crumbs.push_back({ t_position });
			oldBack->setNext(&m_crumbs.back());
		}
		else
		{
			m_crumbs.push_back({ t_position });
		}

		m_canSpawn = false;

		m_crumbs.back().initParticles();

		if (!IsSoundPlaying(m_stepSound))
		{
		}
		PlaySound(m_stepSound);
	}
}

void Breadcrumb::timerUpdate()
{
	if (m_timer < MAX_TIME && !m_canSpawn)
	{
		m_timer += GetFrameTime();
	}
	else
	{
		m_canSpawn = true;
		m_timer = 0.0f;
	}
}

void Breadcrumb::updateCrumbParticles()
{
	for (Crumb& bc : m_crumbs)
	{
		bc.updateParticles();
	}
}
