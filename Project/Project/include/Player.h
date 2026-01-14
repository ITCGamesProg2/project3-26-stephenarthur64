#pragma once
#include "GameObject.h"
class Player :
    public GameObject
{
public:
    Player(Color t_c, float t_r);
    virtual void update() override;
    virtual void move() override;
    void moveDirection(Vector2 t_direction);
};

