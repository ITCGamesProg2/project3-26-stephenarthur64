#pragma once
#include "Attack.h"
class LightAttack :
    public Attack
{
public:
    LightAttack(Vector2 t_start);
    void setEnd(Vector2 t_end) { m_endPos = t_end; }
    void setStart(Vector2 t_start) { m_startPos = t_start; }
    virtual void execute() override;
    virtual void process() override;
    virtual void draw() override;
};

