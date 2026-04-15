#pragma once
#include "Boss.h"
#include "SpecialAttack.h"

class StopBoss :
    public Boss
{
public:
    StopBoss();
    virtual void draw() override;
    virtual void update() override;
    virtual void immuneCheck(Vector2 t_playerPos) override;
    void setTarget(Vector2 t_target) override {}
};

