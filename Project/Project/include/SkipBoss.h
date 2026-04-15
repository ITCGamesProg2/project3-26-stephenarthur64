#pragma once
#include "Boss.h"
#include "HeavyAttack.h"

class SkipBoss :
    public Boss
{
public:
    SkipBoss();
    virtual void draw() override;
    virtual void immuneCheck(Vector2 t_playerPos) override;
};

