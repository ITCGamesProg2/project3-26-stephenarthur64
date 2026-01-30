#pragma once
#include "NPC.h"
#include "HeavyAttack.h"

class EnemyHeavy :
    public NPC
{
public:
    EnemyHeavy(Color t_c, float t_r) : NPC(t_c, t_r)
    {
        m_attack = new HeavyAttack({-1000, -1000});
        m_attack->setCooldownScale(10.0f);
        m_attack->setWindupScale(5.0f);
    }
};

