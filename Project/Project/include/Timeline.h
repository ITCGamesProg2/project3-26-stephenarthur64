#pragma once
#include "raylib.h"
#include "globals.h"
#include <list>

static class Timeline
{
public:
	static void addTime(Time t_new);
	static Time rewind();
	static void drawTimeline();
	static bool canRewind();
	static void clearTimeline();
};

