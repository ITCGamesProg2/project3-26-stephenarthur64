#pragma once
#include <raylib.h>
#include "globals.h"

class Pickup
{
public:
    Pickup();
    void draw();
    void setPosition(Vector2 t_pos);
    void setAbility(TimeAbilities t_a) { m_ability = t_a; }

    Vector2 getPosition() { return m_position; }
    float getRadius() { return m_radius; }
    TimeAbilities getAbility() { return m_ability; }
    void deactivate() { m_alive = false; }
    bool isAlive() { return m_alive;  }

private:
    TimeAbilities m_ability;
    Vector2 m_position;
    float m_radius;
    bool m_alive;
};

