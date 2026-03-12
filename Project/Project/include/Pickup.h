#pragma once
#include <raylib.h>
#include "globals.h"
#include "AssetManager.h"

class Pickup
{
public:
    Pickup();
    void draw();
    void setPosition(Vector2 t_pos);
    void setAbility(TimeAbilities t_a);
    void setSprite() { m_sprite = AssetManager::getSprite("powers"); }

    Vector2 getPosition() { return m_position; }
    float getRadius() { return m_radius; }
    TimeAbilities getAbility() { return m_ability; }
    void deactivate();
    bool isAlive() { return m_alive;  }

private:
    TimeAbilities m_ability;
    Vector2 m_position;
    float m_radius;
    bool m_alive;
    Texture2D m_sprite;
    int m_frame;
};

