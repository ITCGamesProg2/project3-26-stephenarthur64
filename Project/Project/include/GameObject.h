#pragma once
#include "raylib.h"
#include "raymath.h"

class GameObject
{
public:
	GameObject(Color t_c, float t_radius);
	virtual void draw();
	virtual void update() = 0;
	virtual void move() = 0;
	virtual void collision (bool t_damage, Vector2 t_pos) = 0;
	virtual void applyKnockback(Vector2 t_collidePos);
	Vector2 getPosition() { return m_position; }
	float getRadius() { return m_radius; }

protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;
	float m_speed;
	Color m_colour;
};

