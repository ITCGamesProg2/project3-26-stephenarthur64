#pragma once
#include "GameObject.h"
#include "globals.h"
#include "Timeline.h"
#include "LightAttack.h"
#include "HeavyAttack.h"
#include "CollisionCheck.h"

class Player :
    public GameObject
{
public:
    Player(Color t_c, float t_r);
    virtual void update() override;
    virtual void move() override;
    virtual void draw() override;
    virtual void collision (bool t_damage, Vector2 t_pos) override;
    void addForce(Vector2 t_direction);

    float getRadius() { return m_radius; }

    bool rewind();
    Time generateTime();
    void useAttack(AttackTypes t_attack);

private:
    Time m_newTime;
    float deltaTime;
    const float MAX_SPEED;
    const float MIN_SPEED;

    LightAttack m_light;
    HeavyAttack m_heavy;
};

