#pragma once
#include "NPC.h"
#include "Pickup.h"

class Boss :
    public NPC
{
public:
    Boss();
    virtual void move() override;
    virtual void draw() override;
    virtual void update() override;
    virtual void damage(int t_amount) override;
    void immuneCheck(Vector2 t_playerPos);

    Pickup& getUpgrade() { return m_upgrade; }

protected:
    bool m_canDamage;
    Pickup m_upgrade;
};

