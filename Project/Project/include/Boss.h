#pragma once
#include "NPC.h"
#include "Pickup.h"

class Boss :
    public NPC
{
public:
    Boss();
    virtual void draw() = 0;
    virtual void immuneCheck(Vector2 t_playerPos) = 0;
    virtual void update() override;
    virtual void damage(int t_amount) override;
    virtual void setSprite(Texture2D* t_sp) override;

    Pickup& getUpgrade() { return m_upgrade; }

protected:
    bool m_canDamage;
    Pickup m_upgrade;
};

