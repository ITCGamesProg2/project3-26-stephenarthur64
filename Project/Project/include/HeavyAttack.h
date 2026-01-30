#pragma once
#include "Attack.h"

class HeavyAttack :
    public Attack
{
public:
    HeavyAttack(Vector2 t_start)
    {
		m_running = false;
		m_startPos = t_start;
		m_width = 100.0f;
		m_height = 100.0f;
		m_speed = 7.0f;
		m_maxWindup = 1.0f;
		m_maxDuration = 0.5f;
		m_maxCooldown = 1.0f;
		m_cooldown = m_maxCooldown;
		m_hitboxOffset = 25.0f;
		m_type = HEAVY;
		m_damage = 2;
    }
};

