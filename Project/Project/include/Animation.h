#pragma once
#include "raylib.h"

class Animation
{
public:
	Animation(int t_max, float t_x, float t_y);
	void nextFrame(float t_frameTime);
	Rectangle getFrame();
	void flipSprite(int t_direction) 
	{ 
		m_direction = t_direction; 
	}

private:
	int m_frame;
	const int MAX_FRAMES;
	float m_frameTime;
	const float MAX_FRAME_TIME;

	float m_sizeX;
	float m_sizeY;
	int m_direction;
};

