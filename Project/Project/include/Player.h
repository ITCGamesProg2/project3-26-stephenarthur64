#pragma once
#include "GameObject.h"
#include "globals.h"
#include "reasings.h"

class Player :
    public GameObject
{
public:
    Player(Color t_c, float t_r);
    virtual void update() override;
    virtual void move() override;
    void moveDirection(Vector2 t_direction);

    float getRadius() { return m_radius; }

    void rewind(Time t_new);

private:
    Vector2 m_positionBuffer;
    float deltaTime;
    const float MAX_SPEED;
    const float MIN_SPEED;
};

