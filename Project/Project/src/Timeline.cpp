#include "Timeline.h"

Timeline::Timeline() : MAX_TIME(30)
{
}

void Timeline::addTime(Time t_new)
{
	if (m_timeline.size() >= MAX_TIME)
	{
		m_timeline.pop_front();
	}
	m_timeline.push_back(t_new);
}

Time Timeline::rewind()
{
	if (m_timeline.size() == 0)
	{
		return Time();
	}

	Time temp = m_timeline.back();
	m_timeline.pop_back();

	return temp;
}

void Timeline::drawTimeline()
{
	for (Time t : m_timeline)
	{
		DrawCircle(t.position.x, t.position.y, t.radius, { 173, 216, 230, 50 });
	}
}
