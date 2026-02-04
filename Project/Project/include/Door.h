#pragma once
#include "Wall.h"
#include "NPC.h"
#include <vector>

class Door :
    public Wall
{
public:
    Door(Color t_c, float t_x, float t_y);
    virtual void draw() override;
    virtual void update() override;
    void open();
    void addEnemy(NPC* t_e);

private:
    std::vector<NPC*> m_enemies;
    int m_enemyCount;
    int m_enemyMax;
};

