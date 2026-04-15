#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"
#include <fstream>
#include <json.hpp>
#include <iostream>

Game::Game() : m_player(BLUE, 35.0f), m_rewinding(false), REWIND_INTERVAL(0.2), m_rewindTimer(0.0f), TIME_STOP_MAX(2), m_state(GameState::MENU), m_skipCount(0), m_surpriseTimer(0), 
                m_skipColours(0), SKIP_MAX(1.0f), m_placePos({INFINITY, 0.0f}), m_placing(false), REWIND_MAX(1.0f), STOP_MAX(1.5F), m_musicPos(0.0f), added(false), m_paused(false),
                m_editing(false)
{
}

void Game::init()
{
    LevelLoader::initGrid();

    m_background = LoadTexture("img/Environment/floor.png");
    m_camera.zoom = 1.0f;
    m_camTarget = m_player.getPosition();

    AssetManager::initAssets();
    m_menu.init();

    m_powersSprite = &AssetManager::getSprite("powers");
    m_timeStopSound = &AssetManager::getSound("timestop");
    m_timeStopEndSound = &AssetManager::getSound("timestopend");

    m_timeStopShader = AssetManager::getShader("stop");
    m_rewindShader = AssetManager::getShader("rewind");
    m_skipShader = AssetManager::getShader("skip");

    target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    LevelLoader::setPlayerRef(&m_player);

    for (int i = 0; i < 3; i++)
    {
        m_menuButtons[i].setSize({ 100.0f, 50.0f });
        m_menuButtons[i].setPosition({ SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / 2.0f) + (75.0f * i) });
    }

    m_menuButtons[0].setText("Continue");
    m_menuButtons[1].setText("Options");
    m_menuButtons[2].setText("Quit");

    for (int i = 0; i < 2; i++)
    {
        m_deathButtons[i].setSize({ 100.0f, 50.0f });
        m_deathButtons[i].setPosition({ SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / 2.0f) + (75.0f * i) });
    }

    m_deathButtons[0].setText("Retry");
    m_deathButtons[1].setText("Quit");

    m_editButton.setSize({ 100.0f, 50.0f });
    m_editButton.setPosition({ 100.0f, SCREEN_HEIGHT - 100.0f });
    m_editButton.setText("Edit");
}

void Game::loadLevel()
{
    resetGame();

    Vector2 bossPos;
    TimeAbilities bossType = TimeAbilities::MAX;

    m_startingPickup.setAbility(TimeAbilities::MAX);

    LevelLoader::LoadLevel(m_walls, m_goals, m_enemies, m_supports, m_doors, bossType, bossPos, m_tutorials, m_startingPickup);

    if (m_startingPickup.getAbility() != TimeAbilities::MAX)
    {
        m_startingPickup.setSprite();
    }

    m_editor.setSpawn(m_player.getPosition());

    m_editor.setWallReference(&m_walls);
    m_editor.setDoorReference(&m_doors);
    m_editor.setGoalReference(&m_goals);
    m_editor.setNPCReference(&m_enemies);
    m_editor.setSupportReference(&m_supports);

    if (LevelLoader::isAtEnd())
    {
        m_state = GameState::END;
        m_camera.target = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
        endGame();
    }

    m_player.setSprite(&AssetManager::getSprite("player"));

    for (NPC& e : m_enemies)
    {
        switch (e.getType())
        {
        case LIGHT:
            e.setSprite(&AssetManager::getSprite("light"));
            break;
        case HEAVY:
            e.setSprite(&AssetManager::getSprite("heavy"));
            break;
        default:
            break;
        }
    }

    for (EnemySupport& es : m_supports)
    {
        es.setSprite(&AssetManager::getSprite("support"));
    }

    m_rewindBoss.setSprite(&AssetManager::getSprite("rewindboss"));

    selectBoss(bossType);

    if (m_activeBoss)
    {
        m_boss->setPosition(bossPos);
        m_doors.back().addEnemy(m_boss);
        m_currentMusic = &AssetManager::getMusic("boss");
    }
    else
    {
        m_currentMusic = &AssetManager::getMusic("main");
    }
}

void Game::draw()
{
    if (m_state == GameState::MENU)
    {
        m_menu.draw();
    }

    if (m_state == GameState::GAMEPLAY || m_state == GameState::DEATH || m_state == GameState::EDITING || m_state == GameState::PAUSED)
    {
        BeginTextureMode(target);
        ClearBackground(BLACK);
        BeginMode2D(m_camera);
        DrawTexturePro(m_background, { 0, 0, 640, 640 }, { 0, 0, 5000, 5000 }, { 0,0 }, 0.0f, WHITE);

        if (m_activeBoss)
        {
            m_boss->draw();
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

        for (Door& door : m_doors)
        {
            door.draw();
        }

        for (Tutorial& t : m_tutorials)
        {
            t.draw();
        }

        if (m_startingPickup.getAbility() != TimeAbilities::MAX)
        {
            m_startingPickup.draw();
        }

        if (m_placing)
        {
            m_editor.drawPlacing();
        }
        EndMode2D();
        EndTextureMode();

        processShader();
      
        if (m_shaderActive)
        {
            BeginShaderMode(*m_currentShader);
        }

        DrawTexturePro(target.texture, { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height }, { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0.0f, 0.0f }, 0.0f, WHITE);
        BeginMode2D(m_camera);
        if (m_shaderActive)
        {
            EndShaderMode();
            m_shaderActive = false;
        }
        m_player.draw();
        if (m_rewinding)
        {
            Timeline::drawTimeline();
        }
        EndMode2D();


        DrawRectangle(10, 40, 50, 320, BLACK);
        DrawRectanglePro({ 50, 350, 30, m_player.getMomentumPercentage() * 300 }, { 0, 0 }, 180, LIGHTGRAY);
        DrawRectangle(70, 40, 50, 320, BLACK);
        DrawRectanglePro({ 110, 350, 30, m_player.getHealthPercentage() * 300 }, { 0, 0 }, 180, RED);

        if (m_editing)
        {
            m_editButton.draw();
        }

        if (m_player.canUse(TimeAbilities::REWIND))
        {
            if (!m_rewinding)
            {
                DrawTexturePro(*m_powersSprite, { 0, 0, 32, 32 }, { SCREEN_WIDTH - 240, 10, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(*m_powersSprite, { 0, 0, 32, 32 }, { SCREEN_WIDTH - 240, 10, 70, 70 }, { 0, 0 }, 0.0f, DARKBLUE);
            }
        }

        if (m_player.canUse(TimeAbilities::SKIP))
        {
            if (m_surpriseTimer <= 0)
            {
                DrawTexturePro(*m_powersSprite, { 32, 0, 32, 32 }, { SCREEN_WIDTH - 160, 10, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(*m_powersSprite, { 32, 0, 32, 32 }, { SCREEN_WIDTH - 160, 10, 70, 70 }, { 0, 0 }, 0.0f, RED);
            }
        }

        if (m_player.canUse(TimeAbilities::STOP))
        {
            if (!m_timestop)
            {
                DrawTexturePro(*m_powersSprite, { 64, 0, 32, 32 }, { SCREEN_WIDTH - 80, 10, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(*m_powersSprite, { 96, 0, 32, 32 }, { SCREEN_WIDTH - 80, 10, 70, 70 }, { 0, 0 }, 0.0f, YELLOW);
            }
        }

        if (m_state == GameState::EDITING)
        {
            BeginMode2D(m_camera);
            DrawRectangleLinesEx({ 0, 0, 5000, 5000 }, 2.0f, RED);
            DrawCircle(m_editor.getSpawn().x, m_editor.getSpawn().y, 10.0f, BLUE);
            m_editor.drawDebug();
            for (Goal& goal : m_goals)
            {
                goal.draw();
            }
            EndMode2D();
            m_editor.drawUI();
        }

        if (m_state == GameState::PAUSED)
        {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
            
            if (!m_activePopup)
            {
                for (int i = 0; i < 3; i++)
                {
                    m_menuButtons[i].draw();
                }
            }
            for (Tutorial& t : m_tutorials)
            {
                t.drawPopup();
            }

            if (m_activeBoss)
            {
                m_boss->getUpgrade().drawPopup();
            }

            if (m_startingPickup.getAbility() != TimeAbilities::MAX)
            {
                m_startingPickup.drawPopup();
            }
        }

        if (m_state == GameState::DEATH)
        {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });

            DrawText("You died", (SCREEN_WIDTH / 2.0f) - 120, 100, 50, WHITE);

            for (int i = 0; i < 2; i++)
            {
                m_deathButtons[i].draw();
            }
        }

        DrawFPS(0, 0);
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
            if (m_paused)
            {
                m_state = GameState::PAUSED;
            }
            else
            {
                if (m_menu.isEditing())
                {
                    m_editing = true;
                }

                m_state = GameState::GAMEPLAY;
                resetGame();
                loadLevel();
            }
        }
    }
    else if (m_state == GameState::GAMEPLAY)
    {
        if (!IsMusicStreamPlaying(*m_currentMusic))
        {
            PlayMusicStream(*m_currentMusic);
        }
        else
        {
            if (!m_timestop && !IsSoundPlaying(AssetManager::getSound("timestopend")))
            {
                UpdateMusicStream(*m_currentMusic);
            }
        }

        m_player.setMousePosition(m_camera);

        handleInput();
        cameraMove();

        if (m_editing)
        {
            m_editButton.detectHover(GetMousePosition());
        }

        if (m_editButton.triggered() && m_editing)
        {
            m_state = GameState::EDITING;
            m_editButton.resetTrigger();
        }

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
        if (m_editor.resumeTriggered())
        {
            m_state = GameState::GAMEPLAY;
            m_editor.resetResume();
        }
    }
    else if (m_state == GameState::DEATH)
    {
        deadUpdate();
    }
    else if (m_state == GameState::PAUSED)
    {
        pausedUpdate();
    }
}

void Game::resetGame()
{
    if (m_currentMusic != nullptr)
    {
        if (IsMusicStreamPlaying(*m_currentMusic))
        {
            StopMusicStream(*m_currentMusic);
        }
    }
    m_enemies.clear();
    m_supports.clear();
    m_walls.clear();
    m_goals.clear();
    m_doors.clear();
    Timeline::clearTimeline();
}

void Game::standardUpdate()
{
    if (m_timeSkip)
    {
        m_player.addForce(m_playerDirection);
    }

    CheckCollisions();

    m_player.update();
    for (Tutorial& t : m_tutorials)
    {
        t.update();
        if (t.popupAlive())
        {
            m_state = GameState::PAUSED;
            m_activePopup = true;
        }
    }

    if (m_activeBoss)
    {
        m_boss->getUpgrade().updatePopup();

        if (m_boss->getUpgrade().popupAlive())
        {
            m_state = GameState::PAUSED;
            m_activePopup = true;
        }
    }

    if (m_startingPickup.getAbility() != TimeAbilities::MAX)
    {
        m_startingPickup.updatePopup();

        if (m_startingPickup.popupAlive())
        {
            m_state = GameState::PAUSED;
            m_activePopup = true;
        }
    }

    if (m_surpriseTimer > 0)
    {
        m_surpriseTimer -= GetFrameTime();

        if (m_surpriseTimer <= 0)
        {
            for (NPC& e : m_enemies)
            {
                e.unsurprise();
            }
            if (m_activeBoss)
            {
                m_boss->unsurprise();
            }
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
        if (!m_boss->isSurprised())
        {
            m_boss->setTarget(m_player.getPosition());
        }
        m_boss->immuneCheck(m_player.getPosition());
        m_boss->update();
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

    if (m_timeCounting < REWIND_INTERVAL)
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
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < 2; i++)
        {
            m_deathButtons[i].detectClick(GetMousePosition());
        }
    }

    for (int i = 0; i < 2; i++)
    {
        m_deathButtons[i].detectHover(GetMousePosition());
    }

    if (m_deathButtons[MainButtons::PLAY].triggered())
    {
        m_state = GameState::GAMEPLAY;
        m_paused = false;
        m_deathButtons[MainButtons::PLAY].resetTrigger();
        resetGame();
        m_player.respawn();
        loadLevel();
    }
    else if (m_deathButtons[1].triggered())
    {
        m_state = GameState::MENU;

        LevelLoader::clearProgress();
        m_menu.resetMenu();

        m_deathButtons[1].resetTrigger();
    }
}

void Game::pausedUpdate()
{
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < 3; i++)
        {
            m_menuButtons[i].detectClick(GetMousePosition());
        }
    }

    for (int i = 0; i < 3; i++)
    {
        m_menuButtons[i].detectHover(GetMousePosition());
    }

    if (m_menuButtons[MainButtons::PLAY].triggered())
    {
        m_state = GameState::GAMEPLAY;
        m_paused = false;
        m_menuButtons[MainButtons::PLAY].resetTrigger();
    }
    else if (m_menuButtons[MainButtons::OPTIONS].triggered() || m_menuButtons[MainButtons::QUIT].triggered())
    {
        m_state = GameState::MENU;

        LevelLoader::clearProgress();
        m_menu.resetMenu();
        
        if (m_menuButtons[MainButtons::OPTIONS].triggered())
        {
            m_menuButtons[MainButtons::OPTIONS].resetTrigger();
            m_menu.forceOptions();
        }
        else
        {
            m_menuButtons[MainButtons::QUIT].resetTrigger();
        }
    }

    if (m_activeBoss)
    {
        if (m_boss->getUpgrade().closeTriggered())
        {
            m_state = GameState::GAMEPLAY;
            m_activePopup = false;
            m_boss->getUpgrade().disablePopup();
        }

        m_boss->getUpgrade().updatePopup();
    }

    if (m_startingPickup.getAbility() != TimeAbilities::MAX)
    {
        if (m_startingPickup.closeTriggered())
        {
            m_state = GameState::GAMEPLAY;
            m_activePopup = false;
            m_startingPickup.disablePopup();
        }

        m_startingPickup.updatePopup();
    }

    for (Tutorial& t : m_tutorials)
    {
        if (t.closeTriggered())
        {
            m_state = GameState::GAMEPLAY;
            m_activePopup = false;
            t.disable();
        }

        t.update();
    }
    
}

void Game::endGame()
{
    m_state = GameState::MENU;
    LevelLoader::clearProgress();
    m_menu.resetMenu();
    m_menu.endGame();
    m_player.setPosition({ 0.0f, 0.0f });
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
        standardUpdate();
        m_skipCount++;
    }
    
    if (IsMusicStreamPlaying(*m_currentMusic))
    {
        m_musicPos = GetMusicTimePlayed(*m_currentMusic);
        m_musicPos += 1.0f;
        SeekMusicStream(*m_currentMusic, m_musicPos);
    }

    m_timeSkip = false;

    m_surpriseTimer = SKIP_MAX;
}


void Game::timeStoppedUpdate()
{
    m_player.update();
    m_player.decreaseMomentum();
    CheckCollisions();

    if (m_player.getMomentum() <= 0.0f)
    {
        PlaySound(*m_timeStopEndSound);
        m_timestop = false;
    }

    for (NPC& e : m_enemies)
    {
        e.updateInvincibilityFrames();
    }

    for (NPC& es : m_supports)
    {
        es.updateInvincibilityFrames();
    }

    if (m_activeBoss)
    {
        m_boss->updateInvincibilityFrames();
    }
}


void Game::rewindingUpdate()
{
    if (m_rewindTimer >= REWIND_INTERVAL / 6)
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
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && m_editing)
        {
            m_editButton.detectClick(GetMousePosition());
        }

        if (IsKeyReleased(KEY_SPACE) && m_player.canUse(TimeAbilities::STOP))
        {
            if (m_player.canTimeStop() && m_timestop == false)
            {
                m_timestop = true;
                m_shaderRadius = 0.0f;
                m_surpriseTimer = STOP_MAX;
                for (NPC& e : m_enemies)
                {
                    e.surprise();
                }
                PlaySound(*m_timeStopSound);
            }
            else
            {
                if (m_timestop)
                {
                    PlaySound(*m_timeStopEndSound);
                }
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
                    m_boss->surprise();
                }
                m_surpriseTimer = REWIND_MAX;
                if (IsMusicStreamPlaying(*m_currentMusic))
                {
                    if (!added)
                    {
                        AttachAudioStreamProcessor(m_currentMusic->stream, AudioProcessEffectLPF);
                    }
                    added = true;
                }
            }
            return;
        }
        else
        {
            DetachAudioStreamProcessor(m_currentMusic->stream, AudioProcessEffectLPF);
            added = false;
            m_rewinding = false;
            SetMusicPitch(*m_currentMusic, 1.0f);
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

        if (IsKeyReleased(KEY_ESCAPE))
        {
            m_state = GameState::PAUSED;
            m_paused = true;
        }

        m_playerDirection = { 0.0f, 0.0f };

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) // Up
        {
            m_playerDirection.y += -1.0f;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) // Down
        {
            m_playerDirection.y += 1.0f;
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) // Left
        {
            m_playerDirection.x += -1.0f;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) // Right
        {
            m_playerDirection.x += 1.0f;
        }
        m_player.addForce(m_playerDirection);
    }
    else if (m_state == GameState::EDITING)
    {
        m_editor.handleInputs(m_placing, m_camera);

        Vector2 pos = { 0, 0 };

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) // Up
        {
            pos.y = -6.0f;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) // Down
        {
            pos.y = 6.0f;
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) // Left
        {
            pos.x = -6.0f;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) // Right
        {
            pos.x = 6.0f;
        }

        m_player.setPosition(m_player.getPosition() + pos);
        m_camera.target += pos;
    }
}

void Game::CheckCollisions()
{
    // Refactor later to use spacial partitioning

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

    for (Tutorial& t : m_tutorials)
    {
        CollisionCheck::CheckCollisionsGoal(m_player, t);
    }

    if (m_timeSkip)
    {
        return;
    }

    for (NPC& e : m_enemies)
    {
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), e);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), e);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), e);

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

    if (m_activeBoss)
    {
        if (m_boss->getUpgrade().isAlive())
        {
            if (CollisionCheck::CheckCollisionPickup(m_player, m_boss->getUpgrade()))
            {
                m_player.newAbility(m_boss->getUpgrade().getAbility());
                m_boss->getUpgrade().deactivate();
            }
        }

        if (m_boss->isAlive())
        {
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), *m_boss);
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), *m_boss);
            CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), *m_boss);

            CollisionCheck::CheckCollisionAttack(m_boss->getAttack(), m_player);
        }
    }

    if (m_startingPickup.getAbility() != TimeAbilities::MAX && m_startingPickup.isAlive())
    {
        if (CollisionCheck::CheckCollisionPickup(m_player, m_startingPickup))
        {
            m_player.newAbility(m_startingPickup.getAbility());
            m_startingPickup.deactivate();
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
        m_boss = &m_rewindBoss;
        m_activeBoss = true;
        break;
    case SKIP:
        m_boss = &m_rewindBoss;
        m_activeBoss = true;
        break;
    case STOP:
        m_boss = &m_rewindBoss;
        m_activeBoss = true;
        break;
    case MAX:
        m_activeBoss = false;
        break;
    default:
        break;
    }
}

void Game::processShader()
{
    if (m_timestop)
    {
        Vector2 normPos = GetWorldToScreen2D(m_player.getPosition(), m_camera);
        normPos.x /= SCREEN_WIDTH;
        normPos.y /= SCREEN_HEIGHT;
        float location[2] = { abs(normPos.x), abs(normPos.y) };
        SetShaderValue(m_timeStopShader, GetShaderLocation(m_timeStopShader, "circleCentre"), location, SHADER_UNIFORM_VEC2);
        if (m_shaderRadius < 1.5f)
        {
            m_shaderRadius += 0.03f;
        }
        SetShaderValue(m_timeStopShader, GetShaderLocation(m_timeStopShader, "radius"), &m_shaderRadius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_timeStopShader, GetShaderLocation(m_timeStopShader, "grayStrength"), &m_shaderRadius, SHADER_UNIFORM_FLOAT);
        m_currentShader = &m_timeStopShader;
        m_shaderActive = true;
    }
    else if (m_shaderRadius > 0)
    {
        Vector2 normPos = GetWorldToScreen2D(m_player.getPosition(), m_camera);
        normPos.x /= SCREEN_WIDTH;
        normPos.y /= SCREEN_HEIGHT;
        float location[2] = { abs(normPos.x), abs(normPos.y) };
        SetShaderValue(m_timeStopShader, GetShaderLocation(m_timeStopShader, "circleCentre"), location, SHADER_UNIFORM_VEC2);
        m_shaderRadius -= 0.05f;
        SetShaderValue(m_timeStopShader, GetShaderLocation(m_timeStopShader, "radius"), &m_shaderRadius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_timeStopShader, GetShaderLocation(m_timeStopShader, "grayStrength"), &m_shaderRadius, SHADER_UNIFORM_FLOAT);
        m_currentShader = &m_timeStopShader;
        m_shaderActive = true;
    }
    else if (m_rewinding)
    {
        m_currentShader = &m_rewindShader;
        m_shaderActive = true;
    }
    else if (m_skipCount > 0)
    {
        SetShaderValue(m_skipShader, GetShaderLocation(m_skipShader, "timer"), &m_surpriseTimer, SHADER_UNIFORM_FLOAT);
        m_currentShader = &m_skipShader;
        m_shaderActive = true;
        m_skipCount--;
    }
}