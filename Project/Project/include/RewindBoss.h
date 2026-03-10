#pragma once
#include "Boss.h"
#include "HeavyAttack.h"

class RewindBoss :
    public Boss
{
public:
    RewindBoss();
    virtual void draw() override;
};

