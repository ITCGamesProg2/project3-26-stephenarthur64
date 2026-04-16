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
/// Checks collisions between a GameObject and a wall
/// </summary>
/// <param name="t_go1">GameObject to be checked</param>
/// <param name="t_go2">Wall</param>
void CollisionCheck::CheckCollisionsWall(GameObject& t_go, Wall& t_wall, bool t_knockback)
{
    if (CheckCollisionPointRec(t_go.nextPositionX(), t_wall.GetHitbox()))
    {
        t_go.stopVelocity(0);
    }
    if (CheckCollisionCircleRec(t_go.nextPositionFull(), t_go.getRadius(), t_wall.GetHitbox()))
    {
        t_go.stopVelocity(2);
    }
    if (CheckCollisionPointRec(t_go.nextPositionY(), t_wall.GetHitbox()))
    {
        t_go.stopVelocity(1);
    }
    if (CheckCollisionCircleRec(t_go.nextPositionFull(), t_go.getRadius(), t_wall.GetHitbox()))
    {
        t_go.stopVelocity(2);
    }
}

void CollisionCheck::CheckCollisionsGoal(GameObject& t_go, Goal& t_goal)
{
    if (CheckCollisionCircleRec(t_go.getPosition(), t_go.getRadius(), t_goal.GetHitbox()))
    {
        t_goal.collision(0, {0, 0});
    }
}

/// <summary>
/// Checks collisions between a given GameObject and an attack of given type
/// </summary>
/// <param name="t_attack">The attack being checked</param>
/// <param name="t_go">The GameObject being checked</param>
void CollisionCheck::CheckCollisionAttack(Attack* t_attack, GameObject& t_go)
{
    if (CheckCollisionCircleRec(t_go.getPosition(), t_go.getRadius(), t_attack->getHitbox()))
    {
        t_go.collision(t_attack->getDamage(), t_attack->getPosition()); // Collision will damage GameObject
        t_attack->collide();
    }
}

void CollisionCheck::CheckCollisionAttackSpecial(Attack* t_attack, GameObject& t_go, GameObject& t_user)
{
    if (t_attack->getType() == SPECIAL)
    {
        if (CheckCollisionCircles(t_go.getPosition(), t_go.getRadius(), t_attack->getPosition(), t_attack->getRadius()))
        {
            t_go.collision(t_attack->getDamage(), t_attack->getPosition());
            t_user.applyKnockback(t_go.getPosition());
            t_attack->collide();
        }
    }
}

void CollisionCheck::CheckCollisionAttackSpecialWall(Attack* t_attack, Rectangle t_hitbox, GameObject& t_user)
{
    if (t_attack->getType() == SPECIAL)
    {
        if (CheckCollisionCircleRec(t_attack->getPosition(), t_attack->getRadius(), t_hitbox))
        {
            if (t_attack->getPosition().x >= t_hitbox.x + t_hitbox.width)
            {
                t_user.applyKnockbackWall(1, 0);
                std::cout << "Right\n";
            }
            else if (t_attack->getPosition().x < t_hitbox.x)
            {
                t_user.applyKnockbackWall(-1, 0);
                std::cout << "Left\n";
            }

            if (t_attack->getPosition().y >= t_hitbox.y + t_hitbox.height)
            {
                t_user.applyKnockbackWall(0, 1);
                std::cout << "Bottom\n";
            }
            else if (t_attack->getPosition().y < t_hitbox.y)
            {
                t_user.applyKnockbackWall(0, -1);
                std::cout << "Top\n";
            }
            t_attack->collide();
        }
    }
}

void CollisionCheck::setPlayerReference(GameObject *t_player)
{
    m_player = t_player;
}

bool CollisionCheck::CheckCollisionPickup(GameObject& t_go, Pickup& t_p)
{
    return CheckCollisionCircles(t_go.getPosition(), t_go.getRadius(), t_p.getPosition(), t_p.getRadius());
}
