#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

Game::Game() : m_player(BLUE, 35.0f)
{
}

void Game::init()
{
}

void Game::draw()
{
    DrawFPS(0, 0);
    m_player.draw();
}

void Game::update()
{
    handleInput();
    m_player.update();
}

void Game::handleInput()
{
    Vector2 direction = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) // Up
    {
        direction.y += -1.0f;
    }
    else if (IsKeyDown(KEY_S)) // Down
    {
        direction.y += 1.0f;
    }
    else if (IsKeyDown(KEY_A)) // Left
    {
        direction.x += -1.0f;
    }
    else if (IsKeyDown(KEY_D)) // Right
    {
        direction.x += 1.0f;
    }

    m_player.moveDirection(direction);
}
