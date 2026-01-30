#pragma once
#include "GameObject.h"
#include "Attack.h"

class NPC :
    public GameObject
{
public:
    NPC(Color t_c, float t_r);
    virtual void update() override;
    virtual void move() override;
    virtual void draw() override;
    virtual void collision(int t_damage, Vector2 t_pos) override;
    void addForce(float t_amount);
    Attack* getAttack() { return m_attack; }

    void setTarget(Vector2 t_target) { m_target = t_target; }
    void setPosition(Vector2 t_pos) { m_position = t_pos; }

    void setActive(bool t_active) { m_alive = t_active; }

protected:
    Vector2 m_target;
    const float MIN_DISTANCE;
    const float MAX_SPEED;
    const float MIN_SPEED;
    Attack *m_attack;
};

