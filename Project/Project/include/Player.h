#pragma once
#include <vector>
#include <iostream>

#include "GameObject.h"
#include "globals.h"
#include "Timeline.h"
#include "LightAttack.h"
#include "HeavyAttack.h"
#include "SpecialAttack.h"
#include "AssetManager.h"

class Player :
    public GameObject
{
public:
    Player(Color t_c, float t_r);
    virtual void update() override;
    virtual void move() override;
    virtual void draw() override;
    virtual void collision (int t_damage, Vector2 t_pos) override;
    virtual void damage(int t_amount) override;
    virtual void respawn() override;
    void addForce(Vector2 t_direction);
    void addMomentum(float t_amount);
    void decreaseMomentum(float t_amount = 1.0f);

    virtual void setSprite(Texture2D* t_sp) override;
    void setMousePosition(Camera2D& t_cam);
    void setPosition(Vector2 t_pos) { m_position = t_pos; }

    float getRadius() { return m_radius; }
    Attack* getAttack(AttackTypes t_type);
    float const getMomentum() { return m_momentum; }
    float const getMomentumPercentage() { return m_momentum / m_maxMomentum; }
    float const getHealthPercentage() { return m_health / (float)m_maxHealth; }

    bool const canTimeStop() { if (m_momentum == m_maxMomentum) { return true; } return false; }

    bool rewind();
    Time generateTime();
    void useAttack(AttackTypes t_attack);
    void comboCheck(AttackTypes t_attack);
    void addToCombo(Attack* t_attack);

    void newAbility(TimeAbilities t_a);
    bool canUse(TimeAbilities t_a);

private:
    Time m_newTime;
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

    float m_momentum;
    float m_maxMomentum;

    Vector2 m_mouse;

    float m_baseRadius;

    bool m_abilities[TimeAbilities::MAX] = {};
};

#include "CollisionCheck.h"
