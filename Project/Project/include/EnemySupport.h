#pragma once
#include "NPC.h"
#include "SupportAttack.h"
#include <vector>

class EnemySupport :
    public NPC
{
public:
    EnemySupport(Color t_c = RED, float t_r = 20.0f) : NPC(t_c, t_r), m_nearestDist(200)
    {
        m_attack = new SupportAttack({ -1000, 1000 });
        m_attack->setCooldownScale(2.0f);
        m_attack->setWindupScale(2.0f);
    }

    virtual void update() override;

    void findNearest(std::vector<NPC>& t_e);
    Vector2 getTarget() { return m_attack->getTarget(); }

private:
    float m_nearestDist;
    float m_tempDist;
};

