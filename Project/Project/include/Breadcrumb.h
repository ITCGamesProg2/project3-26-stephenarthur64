#pragma once
#include <raylib.h>
#include <list>
#include "Crumb.h"

class Breadcrumb
{
public:
	Breadcrumb();
	void drawCrumbs();
	void spawn(Vector2 t_position);
	void timerUpdate();
	void updateCrumbParticles();
	std::list<Crumb>& getCrumbs() { return m_crumbs; }
	void setStepSound(Sound t_sound) { m_stepSound = t_sound; }

private:
	std::list<Crumb> m_crumbs;
	const float MAX_TIME;
	float m_timer;
	const int MAX_POSITIONS;
	bool m_canSpawn;
	Sound m_stepSound;
};

