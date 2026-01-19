#pragma once
#include "Attack.h"
#include <cmath>

class LightAttack :
    public Attack
{
public:
    LightAttack(Vector2 t_start)
    {
		m_startPos = t_start;
		m_width = 50.0f;
		m_height = 50.0f;
		m_speed = 5.0f;
		m_maxDifference = 90.0f;
		m_minAngle = 0.0f;
		m_maxAngle = m_minAngle + m_maxDifference;
		m_maxCooldown = 0.2f;
		m_cooldown = m_maxCooldown;
		m_hitboxOffset = 50.0f;
		m_type = LIGHT;
    }
};

