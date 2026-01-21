#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "Player.h"
#include "NPC.h"
#include "Timeline.h"
#include "CollisionCheck.h"
#include "globals.h"

class Game
{
public:
    Game();
    void init();
    void draw();
    void update();
    void timeStoppedUpdate();
    void rewindingUpdate();
    void standardUpdate();
    void handleInput();
    void CheckCollisions();
    void loadFile();
    void cameraMove();

private:
    Player m_player;
    NPC m_testnpc;

    Time m_tempTime;
    Time m_newTime;
    bool m_rewinding;
    bool m_timestop;
    float m_timeCounting;
    float m_rewindTimer;
    const float TIME_INTERVAL;
    const float TIME_STOP_MAX;

    Texture2D m_background;
    Camera2D m_camera;
    Vector2 m_camTarget;
};

#endif // GAME_H
