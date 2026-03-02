#pragma once
#include "raylib.h"
#include "raymath.h"

class GameObject
{
public:
	GameObject(Color t_c = RED, float t_radius = 30.0f);
	virtual void draw();
	virtual void update();
	virtual void move() = 0;
	virtual void collision (int t_damage, Vector2 t_pos) = 0;
	virtual void applyKnockback(Vector2 t_collidePos, float t_force = 10.0f);
	virtual Vector2 nextPositionX();
	virtual Vector2 nextPositionY();
	virtual Vector2 nextPositionFull();
	void stopVelocity(int t_direction);
	virtual void damage(int t_amount);
	Vector2 getPosition() { return m_position; }
	float getRadius() { return m_radius; }
	bool isAlive() { return m_alive; }
	virtual void respawn();

	void setSprite(Texture2D& t_sp) { m_sprite = t_sp; m_spriteSource = { 0, 0, (float)m_sprite.width, (float)m_sprite.height }; m_bufferSourceWidth = m_sprite.width; }

protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;
	float m_speed;
	Color m_colour;
	int m_health;
	int m_maxHealth;
	float m_invincibilityFrames;
	float m_maxIFrames;
	bool m_alive;
	Texture2D m_sprite;

	Rectangle m_spriteSource;
	int m_bufferSourceWidth;
};

