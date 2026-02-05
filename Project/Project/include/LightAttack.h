#pragma once
#include "Attack.h"
#include <cmath>

class LightAttack :
    public Attack
{
public:
    LightAttack(Vector2 t_start)
    {
		m_running = false;
		m_startPos = t_start;
		m_width = 100.0f;
		m_height = 75.0f;
		m_speed = 5.0f;
		m_maxWindup = 0.1f;
		m_maxDuration = 0.5f;
		m_maxCooldown = 0.2f;
		m_cooldown = m_maxCooldown;
		m_hitboxOffset = 50.0f;
		m_type = LIGHT;
		m_damage = 1;
    }
};

