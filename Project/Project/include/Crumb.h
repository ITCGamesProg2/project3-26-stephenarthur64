#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Particle {
	Vector2 position;
	Vector2 velocity;
	float radius;
	int alpha;
	bool active;
};

class Crumb
{
public:
	Crumb(Vector2 t_position);
	void initParticles();
	void updateParticles();
	void drawParticles();
	Crumb* getNext() { return m_next; }
	void setNext(Crumb* t_next) { m_next = t_next; }
	Vector2 getPosition() { return m_position; }

private:
	Vector2 m_position;
	Crumb* m_next;
	std::vector<Particle> m_particles;
	const int MAX_PARTICLES;
};

