#pragma once
#include "NPC.h"
class Boss :
    public NPC
{
public:
    Boss();
    virtual void move() override;
};

