#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"
#include <fstream>

Game::Game() : m_player(BLUE, 35.0f), m_testnpc(RED, 30.0f), m_rewinding(false), TIME_INTERVAL(0.2), m_rewindTimer(0.0f), TIME_STOP_MAX(2)
{
    m_testnpc.setPosition({ 600.0f, 200.0f });
}

void Game::init()
{
    m_background = LoadTexture("img/backgroundtemp.png");
    m_camera.zoom = 1.0f;
    m_camTarget = m_player.getPosition();
}

void Game::draw()
{
    BeginMode2D(m_camera);
    DrawFPS(0, 0);
    if (m_rewinding)
    {
        DrawTexture(m_background, 0, 0, DARKBLUE);
    }
    else if (m_timestop)
    {
        DrawTexture(m_background, 0, 0, GRAY);
    }
    else
    {
        ClearBackground(WHITE);
        DrawTexture(m_background, 0, 0, WHITE);
    }
    m_player.draw();

    m_testnpc.draw();

    if (m_rewinding)
    {
        Timeline::drawTimeline();
    }

    EndMode2D();

    DrawRectangle(10, 40, 50, 320, BLACK);
    DrawRectangle(20, 50, 30, (m_player.getMomentum() / 100) * 300, LIGHTGRAY);
}

void Game::update()
{
    m_player.setMousePosition(m_camera);
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
}

void Game::standardUpdate()
{
    m_player.update();

    m_testnpc.setTarget(m_player.getPosition());
    m_testnpc.update();

    if (m_timeCounting < TIME_INTERVAL)
    {
        m_timeCounting += GetFrameTime();
    }
    else
    {
        m_timeCounting = 0.0f;
        Timeline::addTime(m_player.generateTime());
    }
}


void Game::timeStoppedUpdate()
{
    m_player.update();
    m_player.decreaseMomentum();

    if (m_player.getMomentum() <= 0.0f)
    {
        m_timestop = false;
    }

    /*if (m_timeCounting < TIME_STOP_MAX)
    {
        m_timeCounting += GetFrameTime();
    }
    else
    {
        m_timestop = false;
        m_timeCounting = 0.0f;
    }*/
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
    if (IsKeyReleased(KEY_SPACE))
    {
        if (m_player.canTimeStop() && m_timestop == false)
        {
            m_timestop = true;
        }
        else
        {
            m_timestop = false;
        }
    }

    if (IsKeyDown(KEY_Q) && !m_timestop)
    {
        if (Timeline::canRewind() && m_player.getMomentum() > 5.0f)
        {
            m_rewinding = true;
        }
        return;
    }
    else
    {
        m_rewinding = false;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        m_player.useAttack(AttackTypes::LIGHT);
    }
    else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        m_player.useAttack(AttackTypes::HEAVY);
    }
    else if (IsKeyDown(KEY_R))
    {
        m_player.useAttack(AttackTypes::SPECIAL);
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

void Game::CheckCollisions()
{
    CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), m_testnpc);
    CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), m_testnpc);
    CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), m_testnpc);

    CollisionCheck::CheckCollisionAttack(m_testnpc.getAttack(), m_player);
}

void Game::loadFile()
{
    std::ifstream file("combos.json");

    std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    
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

   /* if (Vector2DistanceSqr(m_camTarget, m_player.getPosition()) < 10000)
    {
        Vector2 velocity = GetScreenToWorld2D(GetMousePosition(), m_camera) - m_camTarget;
        velocity = Vector2Normalize(velocity);
        velocity *= 5.0f;
        m_camTarget += velocity;
    }*/
}
