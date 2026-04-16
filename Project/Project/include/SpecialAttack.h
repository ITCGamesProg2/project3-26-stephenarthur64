#pragma once
#include "Attack.h"
class SpecialAttack :
    public Attack
{
public:
    SpecialAttack(Vector2 t_start)
    {
        m_startPos = t_start;
        m_radius = 80.0f;
        m_speed = 7.0f;
        m_maxCooldown = 2.0f;
        m_cooldown = 0.0f;
        m_maxWindup = 0.1f;
        m_maxDuration = 2.0f;
        m_hitboxOffset = 25.0f;
        m_type = SPECIAL;
        m_damage = 1;
        m_windup = 0.0f;
        m_duration = 0.0f;
        m_collided = false;
    }
    virtual void draw() override
    {
        if (m_running) { DrawCircleV(m_startPos, m_radius, YELLOW); }
    }

    virtual bool canAttack() override
    {
        if (m_running || m_cooldown <= m_maxCooldown)
        {
            return false;
        }
        if (!m_running && m_cooldown < m_maxCooldown)
        {
            return false;
        }
        return true;
    }

    virtual void process()
    {
		if (m_running)
		{
			m_windup += GetFrameTime();

			if (m_windup >= m_maxWindup)
			{
				m_duration += GetFrameTime();
				//m_animation.nextFrame(GetFrameTime());

				if (m_duration >= m_maxDuration)
				{
					m_running = false;
					m_windup = 0.0f;
					//m_animation.resetTime();
				}
			}
		}
		else
		{
			m_cooldown += GetFrameTime();
		}
    }

    float getRadius() { return m_radius; }
};

