#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "MainMenu.h"
#include "Player.h"
#include "EnemyLight.h"
#include "EnemyHeavy.h"
#include "Wall.h"
#include "Timeline.h"
#include "CollisionCheck.h"
#include "globals.h"

enum GameState {
    MENU,
    GAMEPLAY
};

class Game
{
public:
    Game();
    void init();
    void loadLevel(int t_level);
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
    GameState m_state;
    MainMenu m_menu;

    Player m_player;
    std::vector<EnemyLight> m_light;
    std::vector<EnemyHeavy> m_heavy;

    std::vector<Wall> m_walls;

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
