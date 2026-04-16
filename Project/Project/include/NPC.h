#pragma once
#include "GameObject.h"
#include "Attack.h"
#include "Breadcrumb.h"
#include <iostream>

class NPC :
    public GameObject
{
public:
    NPC(Color t_c, float t_r);
    ~NPC();
    virtual void update() override;
    virtual void move() override;
    virtual void draw() override;
    virtual void collision(int t_damage, Vector2 t_pos) override;
    void addForce(float t_amount);
    Attack* getAttack() { return m_attack; }
    AttackTypes getType() { return m_attackType; }

    void decideTarget();
    virtual void setPlayerTarget(Vector2 t_target) { m_playerTarget = t_target; }
    void setBreadcrumbsTarget(std::list<Crumb>& t_bc) { if (m_breadcrumb == nullptr) { m_breadcrumb = &t_bc; } }
    void setPosition(Vector2 t_pos) { m_position = t_pos; }

    void setActive(bool t_active) { m_alive = t_active; }
    bool isSurprised() { return m_surprise; }
    void surprise() { m_surprise = true; }
    void unsurprise() { m_surprise = false; }
    void heal();
    void select() { m_selected = true; }
    void deselect() { m_selected = false; }

protected:
    Vector2 m_playerTarget;
    Vector2 m_target;
    float m_distToTarget;
    float m_minDistance;
    const float MAX_SPEED;
    const float MIN_SPEED;
    Attack *m_attack;
    bool m_surprise;
    float m_healTimer;
    const float MAX_HEAL_TIMER;
    AttackTypes m_attackType;
    bool m_selected;
    std::list<Crumb>* m_breadcrumb;
    Crumb* m_targetCrumb;
};

