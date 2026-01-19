#pragma once
#include <vector>

#include "GameObject.h"
#include "globals.h"
#include "Timeline.h"
#include "LightAttack.h"
#include "HeavyAttack.h"
#include "SpecialAttack.h"
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
    void comboCheck(AttackTypes t_attack);
    void addToCombo(Attack* t_attack);

private:
    Time m_newTime;
    float deltaTime;
    const float MAX_SPEED;
    const float MIN_SPEED;

    int m_comboCurrent;
    const int COMBO_MAX;

    float m_comboCountdown;
    const float COMBO_MAX_COOLDOWN;

    float m_friction;
    const float DEFAULT_FRICTION;

    LightAttack m_light;
    HeavyAttack m_heavy;
    SpecialAttack m_special;

    std::vector<Attack*> m_combo;
};

