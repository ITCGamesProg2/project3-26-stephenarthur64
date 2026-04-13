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
    void setTutorialText(std::string t_text) { m_popup.setText(t_text); }
    void drawPopup();

private:
    Popup m_popup;
};

