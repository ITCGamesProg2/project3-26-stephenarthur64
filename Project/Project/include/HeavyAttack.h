#pragma once
#include "Attack.h"

class HeavyAttack :
    public Attack
{
public:
    HeavyAttack(Vector2 t_start)
    {
		m_startPos = t_start;
		m_width = 100.0f;
		m_height = 100.0f;
		m_speed = 7.0f;
		m_maxDifference = 90.0f;
		m_minAngle = 0.0f;
		m_maxAngle = m_minAngle + m_maxDifference;
		m_maxCooldown = 1.0f;
		m_cooldown = m_maxCooldown;
		m_hitboxOffset = 25.0f;
    }
};

