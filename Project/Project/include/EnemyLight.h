#pragma once
#include "NPC.h"
#include "LightAttack.h"

class EnemyLight :
    public NPC
{
public:
    EnemyLight(Color t_c = RED, float t_r = 30.0f) : NPC(t_c, t_r)
    {
        m_attack = new LightAttack({-1000, 1000});
        m_attack->setCooldownScale(2.0f);
        m_attack->setWindupScale(2.0f);
    }
};

