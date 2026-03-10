#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"
#include <fstream>
#include <json.hpp>
#include <iostream>

Game::Game() : m_player(BLUE, 35.0f), m_rewinding(false), TIME_INTERVAL(0.2), m_rewindTimer(0.0f), TIME_STOP_MAX(2), m_state(GameState::MENU), m_skipCount(0), m_surpriseTimer(0), 
                m_skipColours(0), SKIP_MAX(1.0f), m_placePos({INFINITY, 0.0f}), m_placing(false), REWIND_MAX(1.0f), STOP_MAX(1.5F)
{
}

void Game::init()
{
    m_background = LoadTexture("img/Environment/floor.png");
    m_camera.zoom = 1.0f;
    m_camTarget = m_player.getPosition();

    AssetManager::initAssets();

    AssetManager::setVolume(0.3f);
}

void Game::loadLevel()
{
    resetGame();

    Vector2 bossPos;
    TimeAbilities bossType = TimeAbilities::MAX;

    LevelLoader::LoadLevel(m_walls, m_goals, m_enemies, m_supports, m_doors, m_player, bossType, bossPos);

    Editor::setWallReference(&m_walls);
    Editor::setDoorReference(&m_doors);
    Editor::setGoalReference(&m_goals);
    Editor::setNPCReference(&m_enemies);
    Editor::setSupportReference(&m_supports);

    if (LevelLoader::isAtEnd())
    {
        m_state = GameState::END;
    }

    m_player.setSprite(AssetManager::getSprite("player"));

    for (NPC& e : m_enemies)
    {
        switch (e.getType())
        {
        case LIGHT:
            e.setSprite(AssetManager::getSprite("light"));
            break;
        case HEAVY:
            e.setSprite(AssetManager::getSprite("heavy"));
            break;
        default:
            break;
        }
    }

    for (EnemySupport& es : m_supports)
    {
        es.setSprite(AssetManager::getSprite("support"));
    }

    selectBoss(bossType);

    if (m_activeBoss)
    {
        m_boss.get()->setPosition({ 100, 400 });
    }
}

void Game::draw()
{
    BeginMode2D(m_camera);
    if (m_state == GameState::MENU)
    {
        m_menu.draw();
    }
    else if (m_state == GameState::GAMEPLAY || m_state == GameState::DEATH || m_state == GameState::EDITING)
    {
        if (m_rewinding)
        {
            DrawTexturePro(m_background, { 0, 0, 1000, 1000 }, { 0, 0, 100, 100 }, { 0,0 }, 0.0f, WHITE);
        }
        else if (m_timestop)
        {
            DrawTexture(m_background, 0, 0, GRAY);
        }
        else if (m_surpriseTimer > 0)
        {
            DrawTexture(m_background, 0, 0, {255, (unsigned char)m_skipColours,  (unsigned char)m_skipColours, 255});
            m_skipColours += GetFrameTime() * (255 / SKIP_MAX);
        }
        else
        {
            ClearBackground(WHITE);
            DrawTexturePro(m_background, { 0, 0, 640, 640 }, { -1500, -1500, 5000, 5000 }, { 0,0 }, 0.0f, WHITE);
        }
        m_player.draw();

        if (m_activeBoss)
        {
            m_boss.get()->draw();
        }

        for (NPC& e : m_enemies)
        {
            e.draw();
        }

        for (EnemySupport& es : m_supports)
        {
            es.draw();
        }

        for (Wall& wall : m_walls)
        {
            wall.draw();
        }

        for (Goal& goal : m_goals)
        {
            goal.draw();
        }

        for (Door& door : m_doors)
        {
            door.draw();
        }
        
        if (m_rewinding)
        {
            Timeline::drawTimeline();
        }

        if (m_placing)
        {
            Editor::drawPlacing();
        }

        EndMode2D();

        DrawRectangle(10, 40, 50, 320, BLACK);
        DrawRectanglePro({ 50, 350, 30, m_player.getMomentumPercentage() * 300 }, { 0, 0 }, 180, LIGHTGRAY);
        DrawRectangle(70, 40, 50, 320, BLACK);
        DrawRectanglePro({ 110, 350, 30, m_player.getHealthPercentage() * 300 }, { 0, 0 }, 180, RED);


        if (m_player.canUse(TimeAbilities::REWIND))
        {
            if (!m_rewinding)
            {
                DrawTexturePro(AssetManager::getSprite("powers"), { 0, 0, 32, 32 }, { SCREEN_WIDTH - 240, 10, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(AssetManager::getSprite("powers"), { 0, 0, 32, 32 }, { SCREEN_WIDTH - 240, 10, 70, 70 }, { 0, 0 }, 0.0f, DARKBLUE);
            }
        }

        if (m_player.canUse(TimeAbilities::SKIP))
        {
            if (m_surpriseTimer <= 0)
            {
                DrawTexturePro(AssetManager::getSprite("powers"), { 32, 0, 32, 32 }, { SCREEN_WIDTH - 160, 10, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(AssetManager::getSprite("powers"), { 32, 0, 32, 32 }, { SCREEN_WIDTH - 160, 10, 70, 70 }, { 0, 0 }, 0.0f, RED);
            }
        }

        if (m_player.canUse(TimeAbilities::STOP))
        {
            if (!m_timestop)
            {
                DrawTexturePro(AssetManager::getSprite("powers"), { 64, 0, 32, 32 }, { SCREEN_WIDTH - 80, 10, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(AssetManager::getSprite("powers"), { 96, 0, 32, 32 }, { SCREEN_WIDTH - 80, 10, 70, 70 }, { 0, 0 }, 0.0f, YELLOW);
            }
        }

        DrawFPS(0, 0);
        

        if (m_state == GameState::EDITING)
        {
            DrawText(("Room " + std::to_string(Editor::getRoom())).c_str(), 100, 100, 20, BLUE);
            DrawText(Editor::getState().c_str(), 100, 50, 30, BLUE);
        }

        if (m_state == GameState::DEATH)
        {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
            DrawText("Player Dead", (SCREEN_WIDTH / 2.0f), SCREEN_HEIGHT / 2.0f, 20, WHITE);
        }
    }
    else if (m_state == GameState::END)
    {
        DrawText("END OF GAME", 200, 200, 50, WHITE);
    }
}

void Game::update()
{
    if (m_state == GameState::MENU)
    {
        m_menu.update();
        if (m_menu.ended())
        {
            m_state = GameState::GAMEPLAY;
            resetGame();
            loadLevel();
        }
    }
    else if (m_state == GameState::GAMEPLAY)
    {
        if (!IsMusicStreamPlaying(AssetManager::getMusic("main")))
        {
            PlayMusicStream(AssetManager::getMusic("main"));
        }
        else
        {
            UpdateMusicStream(AssetManager::getMusic("main"));
        }

        if (!m_timeSkip)
        {
            m_player.setMousePosition(m_camera);
        }
        handleInput();
        cameraMove();
        CheckCollisions();

        if (m_rewinding)
        {
            rewindingUpdate();
        }
        else if (m_timestop)
        {
            timeStoppedUpdate();
        }
        else
        {
            standardUpdate();
        }

        if (!m_player.isAlive())
        {
            m_state = GameState::DEATH;
        }
    }
    else if (m_state == GameState::EDITING)
    {
        handleInput();
        cameraMove();
    }
    else if (m_state == GameState::DEATH)
    {
        deadUpdate();
    }
    else if (m_state == GameState::END)
    {
        m_camera.target = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
        gameEndUpdate();
    }
}

void Game::resetGame()
{
    StopMusicStream(AssetManager::getMusic("main"));
    m_player.respawn();
    m_enemies.clear();
    m_walls.clear();
    m_goals.clear();
    m_doors.clear();
    Timeline::clearTimeline();
    PlayMusicStream(AssetManager::getMusic("main"));
}

void Game::standardUpdate()
{
    m_player.update();

    if (m_surpriseTimer > 0)
    {
        m_surpriseTimer -= GetFrameTime();

        if (m_surpriseTimer <= 0)
        {
            for (NPC& e : m_enemies)
            {
                e.unsurprise();
            }
            m_boss.get()->unsurprise();
        }
    }

    for (NPC& e : m_enemies)
    {
        if (!e.isSurprised())
        {
            e.setTarget(m_player.getPosition());
        }
        e.update();
    }

    if (m_activeBoss)
    {
        if (!m_boss.get()->isSurprised())
        {
            m_boss.get()->setTarget(m_player.getPosition());
        }
        m_boss.get()->immuneCheck(m_player.getPosition());
        m_boss.get()->update();
    }

    for (EnemySupport& es : m_supports)
    {
        es.update();
        es.findNearest(m_enemies);
    }

    for (Door& d : m_doors)
    {
        d.update();
    }

    if (m_timeCounting < TIME_INTERVAL)
    {
        m_timeCounting += GetFrameTime();
    }
    else
    {
        m_timeCounting = 0.0f;
        Timeline::addTime(m_player.generateTime());
    }

    if (LevelLoader::isNextLevelReady())
    {
        loadLevel();
    }
}

void Game::deadUpdate()
{
    if (IsKeyReleased(KEY_SPACE))
    {
        m_state = GameState::GAMEPLAY;
        resetGame();
        loadLevel();
    }
}

void Game::gameEndUpdate()
{
    if (IsKeyReleased(KEY_SPACE))
    {
        m_state = GameState::MENU;
        LevelLoader::clearProgress();
        m_menu.resetMenu();
        m_player.setPosition({ 0.0f, 0.0f });
    }
}

void Game::timeSkip()
{
    m_skipCount = 0;

    for (NPC& e : m_enemies)
    {
        e.surprise();
    }

    while (m_skipCount < 60)
    {
        update();
        m_skipCount++;
    }

    m_timeSkip = false;

    m_surpriseTimer = SKIP_MAX;
}


void Game::timeStoppedUpdate()
{
    m_player.update();
    m_player.decreaseMomentum();

    if (m_player.getMomentum() <= 0.0f)
    {
        m_timestop = false;
    }
}


void Game::rewindingUpdate()
{
    if (m_rewindTimer >= TIME_INTERVAL / 8)
    {
        m_rewindTimer = 0.0f;
        m_rewinding = m_player.rewind();
    }
    m_rewindTimer += GetFrameTime();
}

void Game::handleInput()
{
    if (m_state == GameState::GAMEPLAY)
    {
        if (IsKeyReleased(KEY_SPACE) && m_player.canUse(TimeAbilities::STOP))
        {
            if (m_player.canTimeStop() && m_timestop == false)
            {
                m_timestop = true;
                m_surpriseTimer = STOP_MAX;
                for (NPC& e : m_enemies)
                {
                    e.surprise();
                }
            }
            else
            {
                m_timestop = false;
            }
        }

        if (IsKeyDown(KEY_Q) && m_player.canUse(TimeAbilities::REWIND) && !m_timestop)
        {
            if (Timeline::canRewind() && m_player.getMomentum() > 5.0f)
            {
                m_rewinding = true;
                for (NPC& e : m_enemies)
                {
                    e.surprise();
                }
                if (m_activeBoss)
                {
                    m_boss.get()->surprise();
                }
                m_surpriseTimer = REWIND_MAX;
                SetMusicPitch(AssetManager::getMusic("main"), 0.90f);
            }
            return;
        }
        else
        {
            m_rewinding = false;
            SetMusicPitch(AssetManager::getMusic("main"), 1.0f);
        }

        if (IsKeyReleased(KEY_R) && m_player.canUse(TimeAbilities::SKIP) && !m_timestop && !m_timeSkip)
        {
            m_timeSkip = true;
            m_player.decreaseMomentum(20.0f);
            timeSkip();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            m_player.useAttack(AttackTypes::LIGHT);
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            m_player.useAttack(AttackTypes::HEAVY);
        }

        Vector2 direction = { 0.0f, 0.0f };

        if (IsKeyDown(KEY_W)) // Up
        {
            direction.y += -1.0f;
        }
        if (IsKeyDown(KEY_S)) // Down
        {
            direction.y += 1.0f;
        }
        if (IsKeyDown(KEY_A)) // Left
        {
            direction.x += -1.0f;
        }
        if (IsKeyDown(KEY_D)) // Right
        {
            direction.x += 1.0f;
        }
        m_player.addForce(direction);
    }
    else if (m_state == GameState::EDITING)
    {
        Editor::handleInputs(m_placing, m_camera);
    }

    if (IsKeyReleased(KEY_B))
    {
        if (m_state == GameState::GAMEPLAY)
        {
            m_state = GameState::EDITING;
        }
        else if (m_state == GameState::EDITING)
        {
            m_state = GameState::GAMEPLAY;
        }
    }
}

void Game::CheckCollisions()
{
    // Refactor later to use spacial partitioning

    for (NPC& e : m_enemies)
    {
        if (!e.isSurprised())
        {
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), e);
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), e);
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), e);
        }

        if (!m_timestop)
        {
            CollisionCheck::CheckCollisionAttack(e.getAttack(), m_player);
        }

        for (EnemySupport& es : m_supports)
        {
            if (CheckCollisionPointCircle(es.getTarget(), e.getPosition(), e.getRadius()))
            {
                e.heal();
            }
        }
    }

    for (EnemySupport& es : m_supports)
    {
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), es);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), es);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), es);
    }
    
    for (Wall& wall : m_walls)
    {
        CollisionCheck::CheckCollisionsWall(m_player, wall, true);

        for (NPC& e : m_enemies)
        {
            CollisionCheck::CheckCollisionsWall(e, wall, true);
        }

        for (EnemySupport& es : m_supports)
        {
            CollisionCheck::CheckCollisionsWall(es, wall, true);
        }
    }

    for (Goal& goal : m_goals)
    {
        CollisionCheck::CheckCollisionsGoal(m_player, goal);
    }

    for (Door& door : m_doors)
    {
        if (door.isAlive())
        {
            CollisionCheck::CheckCollisionsWall(m_player, door, true);

            for (NPC& e : m_enemies)
            {
                CollisionCheck::CheckCollisionsWall(e, door, true);
            }
        }
    }

    if (m_activeBoss)
    {
        if (m_boss.get()->getUpgrade().isAlive())
        {
            if (CollisionCheck::CheckCollisionPickup(m_player, m_boss.get()->getUpgrade()))
            {
                m_player.newAbility(m_boss.get()->getUpgrade().getAbility());
                m_boss.get()->getUpgrade().deactivate();
            }
        }

        if (m_boss.get()->isAlive())
        {
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), *m_boss.get());
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), *m_boss.get());
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), *m_boss.get());

            CollisionCheck::CheckCollisionAttack(m_boss.get()->getAttack(), m_player);
        }
    }
}

void Game::cameraMove()
{
    m_camera.target = m_player.getPosition();
    Vector2 toMouse = GetScreenToWorld2D(GetMousePosition(), m_camera) - m_player.getPosition();
    toMouse = Vector2Normalize(toMouse);
    float scale = Vector2Distance(m_player.getPosition(), GetScreenToWorld2D(GetMousePosition(), m_camera));

    toMouse *= scale / 5.0f;
    m_camTarget = m_player.getPosition() + toMouse;

    m_camera.target = m_camTarget;
    m_camera.offset.x = SCREEN_WIDTH / 2.0f;
    m_camera.offset.y = SCREEN_HEIGHT / 2.0f;
}

void Game::selectBoss(TimeAbilities t_boss)
{
    switch (t_boss)
    {
    case REWIND:
        m_boss = std::make_shared<Boss>(m_rewindBoss);
        m_activeBoss = true;
        break;
    case SKIP:
        m_boss = std::make_shared<Boss>(m_rewindBoss);
        m_activeBoss = true;
        break;
    case STOP:
        m_boss = std::make_shared<Boss>(m_rewindBoss);
        m_activeBoss = true;
        break;
    case MAX:
        m_activeBoss = false;
        break;
    default:
        break;
    }
}
