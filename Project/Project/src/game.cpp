#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

Game::Game() : m_player(BLUE, 35.0f), m_testnpc(RED, 30.0f), m_rewinding(false), TIME_INTERVAL(0.2), m_rewindTimer(0.0f), TIME_STOP_MAX(2)
{
    m_testnpc.setPosition({ 600.0f, 200.0f });
}

void Game::init()
{
}

void Game::draw()
{
    DrawFPS(0, 0);
    if (m_rewinding)
    {
        ClearBackground(DARKBLUE);
    }
    else if (m_timestop)
    {
        ClearBackground(GRAY);
    }
    else
    {
        ClearBackground(WHITE);
    }
    m_player.draw();

    m_testnpc.draw();

    if (m_rewinding)
    {
        m_timeline.drawTimeline();
    }
}

void Game::update()
{
    handleInput();

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
        m_newTime.position = m_player.getPosition();
        m_newTime.radius = m_player.getRadius();
        m_timeline.addTime(m_newTime);
    }
}


void Game::timeStoppedUpdate()
{
    m_player.update();

    if (m_timeCounting < TIME_STOP_MAX)
    {
        m_timeCounting += GetFrameTime();
    }
    else
    {
        m_timestop = false;
        m_timeCounting = 0.0f;
    }
}


void Game::rewindingUpdate()
{
    if (m_rewindTimer >= TIME_INTERVAL / 8)
    {
        m_rewindTimer = 0.0f;
        m_tempTime = m_timeline.rewind();

        if (m_tempTime.position.x != 0.0f)
        {
            m_player.rewind(m_tempTime);
        }
        else
        {
            m_rewinding = false;
            m_rewindTimer = 0.0f;
        }
    }
    m_rewindTimer += GetFrameTime();
}

void Game::handleInput()
{
    if (IsKeyReleased(KEY_SPACE))
    {
        m_timestop = !m_timestop;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !m_timestop)
    {
        if (m_timeline.canRewind())
        {
            m_rewinding = true;
        }
        return;
    }
    else
    {
        m_rewinding = false;
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

    if (direction.x != 0.0f || direction.y != 0.0f)
    {
    }
        m_player.moveDirection(direction);
}
