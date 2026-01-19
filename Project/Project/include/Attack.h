#pragma once
#include "globals.h"
#include "raylib.h"
#include "raymath.h"
#include <list>

class Attack
{
public:
	virtual void execute();
	virtual void process();
	virtual void draw();
	Rectangle getHitbox();
	Vector2 getPosition() { return m_startPos; }
	void setStart(Vector2 t_start) { m_startPos = t_start; }
	float getRadius() { if (m_running) { return m_radius; } return 0.0f; }
	AttackTypes getType() { return m_type; }

	bool isRunning() { return m_running; }
	bool canAttack();

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
	float m_radius;
	AttackTypes m_type;
};

