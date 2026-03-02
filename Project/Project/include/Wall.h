#pragma once
#include "GameObject.h"
#include "AssetManager.h"

class Wall :
    public GameObject
{
public:
    Wall(Color t_c, float t_x, float t_y);
    virtual void move() override;
    virtual void collision(int t_damage, Vector2 t_pos) override;
    virtual void draw() override;

    void setPosition(Vector2 t_pos) { m_position = t_pos; }

    Rectangle GetHitbox() { return { m_position.x, m_position.y, m_sizeX, m_sizeY }; }

protected:
    float m_sizeX;
    float m_sizeY;
};

