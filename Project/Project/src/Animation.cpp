#include "Animation.h"

Animation::Animation(int t_max, float t_x, float t_y, float t_time = 0.2f) : m_maxFrames(t_max), m_maxFrameTime(t_time), m_frame(0), m_sizeX(t_x), m_sizeY(t_y), m_frameTime(0.0f), m_direction(1)
{
}

void Animation::nextFrame(float t_frameTime)
{
	if (m_frameTime > m_maxFrameTime)
	{
		m_frameTime = 0.0f;
		m_frame++;

		if (m_frame >= m_maxFrames)
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

void Animation::setValues(int t_max, float t_x, float t_y, float t_time)
{
	m_maxFrames = t_max;
	m_sizeX = t_x;
	m_sizeY = t_y;
	m_maxFrameTime = t_time;
}
