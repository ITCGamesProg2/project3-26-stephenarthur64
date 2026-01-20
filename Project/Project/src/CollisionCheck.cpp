#include "CollisionCheck.h"

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
/// <param name="t_attack">The attack being checked</param>
/// <param name="t_go">The GameObject being checked</param>
void CollisionCheck::CheckCollisionAttack(Attack* t_attack, GameObject& t_go)
{
    if (t_attack->getType() == SPECIAL)
    {
        if (CheckCollisionCircles(t_go.getPosition(), t_go.getRadius(), t_attack->getPosition(), t_attack->getRadius()))
        {
            t_go.collision(t_attack->getDamage(), t_attack->getPosition());
            m_player->applyKnockback(t_go.getPosition());
            t_attack->collide();
        }
    }
    else
    {
        if (CheckCollisionCircleRec(t_go.getPosition(), t_go.getRadius(), t_attack->getHitbox()))
        {
            t_go.collision(t_attack->getDamage(), t_attack->getPosition()); // Collision will damage GameObject
            t_attack->collide();
        }
    }
 
}

void CollisionCheck::setPlayerReference(GameObject *t_player)
{
    m_player = t_player;
}
