#include "Animation.h"

Animation::Animation(int t_max, float t_x, float t_y) : MAX_FRAMES(t_max), MAX_FRAME_TIME(0.2f), m_frame(0), m_sizeX(t_x), m_sizeY(t_y), m_frameTime(0.0f), m_direction(1)
{
}

void Animation::nextFrame(float t_frameTime)
{
	if (m_frameTime > MAX_FRAME_TIME)
	{
		m_frameTime = 0.0f;
		m_frame++;

		if (m_frame >= MAX_FRAMES)
		{
			m_frame = 0;
		}
	}
	else
	{
		m_frameTime += t_frameTime;
	}
}

Rectangle Animation::getFrame()
{
	return { m_frame * m_sizeX, 0, m_sizeX * m_direction, m_sizeY };
}