#pragma once
#include "raylib.h"

class Attack
{
public:
	virtual void execute() = 0;
	virtual void process() = 0;
	virtual void draw() = 0;

protected:
	bool m_running;
	float m_maxAngle;
	float m_minAngle;
	float m_maxDifference;
	float m_currentAngle;
	Vector2 m_startPos;
	Vector2 m_endPos;
	float m_speed;
	float m_width;
	float m_height;
};

