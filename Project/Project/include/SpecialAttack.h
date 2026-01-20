#pragma once
#include "Attack.h"
class SpecialAttack :
    public Attack
{
public:
    SpecialAttack(Vector2 t_start)
    {
        m_startPos = t_start;
        m_radius = 100.0f;
        m_speed = 7.0f;
        m_maxCooldown = 3.0f;
        m_cooldown = m_maxCooldown;
        m_hitboxOffset = 25.0f;
        m_type = SPECIAL;
        m_damage = 1;
    }
    virtual void draw() override
    {
        if (m_running) { DrawCircleV(m_startPos, m_radius, YELLOW); }
    }

    virtual void process()
    {
        if (m_running)
        {
            m_cooldown += GetFrameTime();

            if (m_cooldown > m_maxCooldown)
            {
                m_cooldown = 0.0f;
                m_running = false;
            }
        }
        else
        {
            m_cooldown = m_maxCooldown;
        }
    }
};

