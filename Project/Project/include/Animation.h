#pragma once
#include "raylib.h"

class Animation
{
public:
	Animation(int t_max, float t_x, float t_y, float t_time);
	void nextFrame(float t_frameTime);
	Rectangle getFrame();
	void resetTime() { m_frame = 0; m_frameTime = 0; }
	void setValues(int t_max, float t_x, float t_y, float t_time);
	void flipSprite(int t_direction) 
	{ 
		m_direction = t_direction; 
	}

private:
	int m_frame;
	int m_maxFrames;
	float m_frameTime;
	float m_maxFrameTime;

	float m_sizeX;
	float m_sizeY;
	int m_direction;
};

