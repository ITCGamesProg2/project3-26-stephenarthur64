#pragma once
#include "globals.h"
#include "raylib.h"
#include "raymath.h"
#include <list>

class Attack
{
public:
	virtual void execute(Vector2 t_target);
	virtual void process();
	virtual void draw();
	void setCooldownScale(float t_scale) { m_maxCooldown *= t_scale; }
	void setWindupScale(float t_scale) { m_maxWindup *= t_scale; }
	Rectangle getHitbox();
	Vector2 getPosition() { return m_startPos; }
	void setStart(Vector2 t_start);
	float getRadius() { if (m_running) { return m_radius; } return 0.0f; }
	AttackTypes getType() { return m_type; }

	bool isRunning() { return m_running; }
	bool canAttack();
	bool isCollided();
	void collide() { m_collided = true; }
	int getDamage() { return m_damage; }

protected:
	bool m_running;
	bool m_collided;

	Vector2 m_startPos;
	Vector2 m_targetPos;

	float m_windup;
	float m_maxWindup;
	float m_duration;
	float m_maxDuration;
	float m_cooldown;
	float m_maxCooldown;

	float m_speed;
	float m_width;
	float m_height;
	float m_hitboxOffset;
	float m_radius;

	AttackTypes m_type;
	int m_damage;
};

