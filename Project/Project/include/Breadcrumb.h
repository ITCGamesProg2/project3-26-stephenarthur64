#pragma once
#include <raylib.h>
#include <list>

struct Crumb {
	Vector2 m_position;
	Crumb* m_next;
};

class Breadcrumb
{
public:
	Breadcrumb();
	void spawn(Vector2 t_position);
	void timerUpdate();
	std::list<Crumb>& getCrumbs() { return m_crumbs; }
private:
	std::list<Crumb> m_crumbs;
	const float MAX_TIME;
	float m_timer;
	const int MAX_POSITIONS;
	bool m_canSpawn;
};

