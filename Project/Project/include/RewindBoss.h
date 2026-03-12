#pragma once
#include "Boss.h"
#include "HeavyAttack.h"

class RewindBoss :
    public Boss
{
public:
    RewindBoss();
    virtual void draw() override;

private:
    Vector2 m_shieldPos;
    Vector2 m_rotateOriginLine;
    Vector2 m_normLineToShield;
    Vector2 m_normTargetToShield;
    float m_shieldAngle;
};

