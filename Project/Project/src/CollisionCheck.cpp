#include "CollisionCheck.h"

static Attack* m_lightAttack;
static Attack* m_heavyAttack;
static Attack* m_specialAttack;
static GameObject* m_player;

/// <summary>
/// Checks Collsions between 2 GameObjects. The first parameter is considered the priority, meaning that if
/// collision is detected, it will damage the second parameter, which is considered secondary
/// </summary>
/// <param name="t_go1">Priority Object</param>
/// <param name="t_go2">Secondary Object</param>
/// <returns></returns>
void CollisionCheck::CheckCollisionsGameObject(GameObject& t_go1, GameObject& t_go2)
{
    bool collision = CheckCollisionCircles(t_go1.getPosition(), t_go1.getRadius(), t_go2.getPosition(), t_go2.getRadius());
    
    if (collision)
    {
        t_go1.collision(false, t_go2.getPosition()); // No damage to priority
        t_go2.collision(true, t_go1.getPosition()); // Damage to secondary
    }
}

/// <summary>
/// Checks collisions between a given GameObject and an attack of given type
/// </summary>
/// <param name="t_type">The type of attack being checked</param>
/// <param name="t_go">The GameObject being checked</param>
void CollisionCheck::CheckCollisionAttack(AttackTypes t_type, GameObject& t_go)
{
    if (t_type == LIGHT)
    {
        if (CheckCollisionCircleRec(t_go.getPosition(), t_go.getRadius(), m_lightAttack->getHitbox()))
        {
            t_go.collision(true, m_lightAttack->getPosition()); // Collision will damage GameObject
        }
    }
    else if (t_type == HEAVY)
    {
        if (CheckCollisionCircleRec(t_go.getPosition(), t_go.getRadius(), m_heavyAttack->getHitbox()))
        {
            t_go.collision(true, m_heavyAttack->getPosition()); // Collision will damage GameObject
        }
    }
    else if (t_type == SPECIAL)
    {
        if (CheckCollisionCircles(t_go.getPosition(), t_go.getRadius(), m_specialAttack->getPosition(), m_specialAttack->getRadius()))
        {
            t_go.collision(true, m_specialAttack->getPosition());
            m_player->applyKnockback(t_go.getPosition());
        }
    }
}

void CollisionCheck::setAttackReference(AttackTypes t_type, Attack* t_attack)
{
    switch (t_type)
    {
    case LIGHT:
        m_lightAttack = t_attack;
        break;
    case HEAVY:
        m_heavyAttack = t_attack;
        break;
    case SPECIAL:
        m_specialAttack = t_attack;
        break;
    default:
        break;
    }
}

void CollisionCheck::setPlayerReference(GameObject *t_player)
{
    m_player = t_player;
}
