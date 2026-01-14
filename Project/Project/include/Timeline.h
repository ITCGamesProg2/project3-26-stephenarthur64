#pragma once
#include "raylib.h"
#include "globals.h"
#include <list>

class Timeline
{
public:
	Timeline();
	void addTime(Time t_new);
	Time rewind();
	void drawTimeline();

private:
	std::list<Time> m_timeline;
	const int MAX_TIME;
};

