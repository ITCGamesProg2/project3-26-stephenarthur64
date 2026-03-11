#ifndef GAME_H
#define GAME_H

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif


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
#include "Editor.h"
#include "RewindBoss.h"

enum class GameState {
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
    void selectBoss(TimeAbilities t_boss);
    void processShader();

private:
    GameState m_state;
    MainMenu m_menu;

    Player m_player;
    std::vector<NPC> m_enemies;
    std::vector<EnemySupport> m_supports;

    std::vector<Wall> m_walls;
    std::vector<Goal> m_goals;
    std::vector<Door> m_doors;

    RewindBoss m_rewindBoss;

    std::shared_ptr<Boss> m_boss;
    bool m_activeBoss;

    Time m_tempTime;
    Time m_newTime;
    bool m_rewinding;
    bool m_timestop;
    bool m_timeSkip;
    float m_timeCounting;
    float m_surpriseTimer;
    const float SKIP_MAX;
    const float REWIND_MAX;
    const float STOP_MAX;
    float m_rewindTimer;
    const float REWIND_INTERVAL;
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

    Shader m_timeStopShader;
    Shader m_rewindShader;
    Shader m_skipShader;
    Shader* m_currentShader;
    bool m_shaderActive;
    RenderTexture2D target;
    float m_shaderRadius;

    float m_musicPos;
    bool added;
};

#endif // GAME_H
