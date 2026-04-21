#pragma once
#include <raylib.h>
#include "globals.h"
#include "AssetManager.h"
#include "Popup.h"

class Pickup
{
public:
    Pickup();
    void draw();
    void setPosition(Vector2 t_pos);
    void setAbility(TimeAbilities t_a);
    void setSprite() { m_sprite = AssetManager::getSprite("powers"); }
    void setTutorials(bool t_tutorials) { m_tutorials = t_tutorials; }

    Vector2 getPosition() { return m_position; }
    float getRadius() { return m_radius; }
    TimeAbilities getAbility() { return m_ability; }
    void deactivate();
    void activate() { m_alive = true; }
    bool isAlive() { return m_alive;  }

    void drawPopup();
    void updatePopup() { m_popup.update(); }
    bool closeTriggered() { return m_popup.closeTriggered(); }
    bool popupAlive() { return m_popup.isAlive(); }
    void disablePopup() { m_popup.disable(); }

private:
    TimeAbilities m_ability;
    Vector2 m_position;
    float m_radius;
    bool m_alive;
    Texture2D m_sprite;
    int m_frame;
    Popup m_popup;
    bool m_tutorials;
};

