#pragma once
#include "raylib.h"
#include "raymath.h"

class Attack
{
public:
	virtual void execute();
	virtual void process();
	virtual void draw();
	Rectangle getHitbox();
	Vector2 getPosition() { return m_startPos; }
	void setStart(Vector2 t_start) { m_startPos = t_start; }

protected:
	bool m_running;
	float m_maxAngle;
	float m_minAngle;
	float m_targetAngle;
	float m_maxDifference;
	float m_currentAngle;
	Vector2 m_startPos;
	Vector2 m_targetPos;
	float m_speed;
	float m_width;
	float m_height;
	int m_direction = 1;
	float m_cooldown;
	float m_maxCooldown;
	float m_hitboxOffset;
};

