#include "Timeline.h"
#include <iostream>

static std::list<Time> m_timeline;
static const int MAX_TIME = 30;
static Texture2D* m_sprite;
static float m_spriteScale;

void Timeline::addTime(Time t_new)
{
	if (m_timeline.size() >= MAX_TIME)
	{
		m_timeline.pop_back();
	}
	m_timeline.push_front(t_new);
}

Time Timeline::rewind()
{
	if (m_timeline.size() == 0)
	{
		return Time();
	}

	Time temp = m_timeline.front();
	m_timeline.pop_front();

	return temp;
}

void Timeline::drawTimeline()
{
	int alpha = 70;
	Vector2 lastpos = {-1000, -1000};

	for (Time &t : m_timeline)
	{
		if (lastpos.x == t.position.x && lastpos.y == t.position.y)
		{
			lastpos = t.position;
			continue;
		}
		DrawTexturePro(*m_sprite, t.spriteRect, { t.position.x, t.position.y, 100 * m_spriteScale, 100 * m_spriteScale }, { 100 * m_spriteScale / 2.0f, 100 * m_spriteScale / 2.0f }, 0, { 173, 216, 230, (unsigned char)alpha });

		lastpos = t.position;
		alpha -= 5;

		if (alpha < 5)
		{
			alpha = 5;
		}
	}
}

bool Timeline::canRewind()
{
	return m_timeline.size() > 5;
}

void Timeline::clearTimeline()
{
	m_timeline.clear();
}

void Timeline::setSpriteInfo(float t_scale, Texture2D* t_sprite)
{
	m_spriteScale = t_scale; 
	m_sprite = t_sprite;
}
