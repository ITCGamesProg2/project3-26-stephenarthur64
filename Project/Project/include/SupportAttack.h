#pragma once
#include "Attack.h"
class SupportAttack :
    public Attack
{
public:
    SupportAttack(Vector2 t_start);

    virtual void execute(Vector2 t_target) override;
    virtual void process() override;
    virtual void draw() override;
};

