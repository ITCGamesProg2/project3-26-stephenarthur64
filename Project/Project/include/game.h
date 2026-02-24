#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "MainMenu.h"
#include "Player.h"
#include "EnemyLight.h"
#include "EnemyHeavy.h"
#include "EnemySupport.h"
#include "Wall.h"
#include "Timeline.h"
#include "CollisionCheck.h"
#include "Goal.h"
#include "Door.h"
#include "LevelLoader.h"
#include "Pickup.h"
#include "globals.h"

enum GameState {
    MENU,
    GAMEPLAY,
    EDITING,
    DEATH,
    END
};

class Game
{
public:
    Game();
    void init();
    void loadLevel();
    void draw();
    void update();
    void resetGame();
    void timeStoppedUpdate();
    void rewindingUpdate();
    void standardUpdate();
    void deadUpdate();
    void gameEndUpdate();
    void timeSkip();
    void handleInput();
    void CheckCollisions();
    void cameraMove();
    void placing();

private:
    GameState m_state;
    MainMenu m_menu;

    Player m_player;
    std::vector<NPC> m_enemies;
    std::vector<EnemySupport> m_supports;

    std::vector<Wall> m_walls;
    std::vector<Goal> m_goals;
    std::vector<Door> m_doors;

    Pickup testpickup;

    Time m_tempTime;
    Time m_newTime;
    bool m_rewinding;
    bool m_timestop;
    bool m_timeSkip;
    float m_timeCounting;
    float m_postSkipTimer;
    const float SKIP_MAX;
    float m_rewindTimer;
    const float TIME_INTERVAL;
    const float TIME_STOP_MAX;

    Texture2D m_background;
    Camera2D m_camera;
    Vector2 m_camTarget;

    int m_levelProgress;

    int m_skipCount;

    float m_skipColours;

    bool m_placing;
    Vector2 m_placePos;
    Vector2 m_placeSize;
    Vector2 m_mousePos;
};

#endif // GAME_H
