#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "Player.h"

class Game
{
public:
    Game();
    void init();
    void draw();
    void update();
    void handleInput();

private:
    Player m_player;
};

#endif // GAME_H
