#pragma once
#include "Goal.h"
#include "Popup.h"
#include "raylib.h"
#include <string>

class Tutorial :
    public Goal
{
public:
    Tutorial(Color t_c, float t_x, float t_y);
    virtual void update() override;
    virtual void collision(int t_damage, Vector2 t_pos) override;
    void setTutorialText(std::string t_key) { m_popup.setTutorial(t_key); }
    void drawPopup();
    bool closeTriggered() { return m_popup.closeTriggered(); }
    bool popupAlive() { return m_popup.isAlive(); }
    void disable() { m_popup.disable(); }

private:
    Popup m_popup;
};

