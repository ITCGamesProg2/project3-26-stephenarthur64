#pragma once
#include "Boss.h"
#include "HeavyAttack.h"

class RewindBoss :
    public Boss
{
public:
    RewindBoss();
    virtual void draw() override;
    void immuneCheck(Vector2 t_playerPos);
};

