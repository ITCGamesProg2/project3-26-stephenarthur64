#pragma once
#include "GameObject.h"
class NPC :
    public GameObject
{
public:
    NPC(Color t_c, float t_r);
    virtual void update() override;
    virtual void move() override;

    void setTarget(Vector2 t_target) { m_target = t_target; }
    void setPosition(Vector2 t_pos) { m_position = t_pos; }

private:
    Vector2 m_target;
    const float MIN_DISTANCE;
};

