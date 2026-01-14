#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "Player.h"
#include "NPC.h"
#include "Timeline.h"
#include "globals.h"

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
    NPC m_testnpc;

    Timeline m_timeline;
    Time m_tempTime;
    Time m_newTime;
    bool m_rewinding;
    bool m_timestop;
    float m_timeCounting;
    float m_rewindTimer;
    const float TIME_INTERVAL;
    const float TIME_STOP_MAX;
};

#endif // GAME_H
