#include "Crumb.h"

Crumb::Crumb(Vector2 t_position) : m_next(nullptr), MAX_PARTICLES(5)
{
	m_position = t_position;
	
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		m_particles.push_back({ m_position });
	}
}

void Crumb::initParticles()
{
	for (Particle& p : m_particles)
	{
		p.position = m_position;
		p.radius = 2.0f;
		p.alpha = 255;

		int angle = rand() % 360;
		angle *= DEG2RAD;

		p.velocity.x = cos(angle);
		p.velocity.y = sin(angle);
		p.active = true;
	}
}

void Crumb::updateParticles()
{
	for (Particle& p : m_particles)
	{
		if (p.active)
		{
			p.position += p.velocity * 1.0f;
			p.alpha -= 4.0f;
			p.radius += 1.0f;

			if (p.alpha < 10)
			{
				p.active = false;
			}
		}
	}
}

void Crumb::drawParticles()
{
	for (Particle& p : m_particles)
	{
		if (p.active)
		{
			DrawRectangle(p.position.x, p.position.y, p.radius, p.radius, { 128, 128, 128, (unsigned char)p.alpha });
		}
	}
}
