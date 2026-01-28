#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"
#include <fstream>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

Game::Game() : m_player(BLUE, 35.0f), m_rewinding(false), TIME_INTERVAL(0.2), m_rewindTimer(0.0f), TIME_STOP_MAX(2), m_state(MENU)
{
}

void Game::init()
{
    m_background = LoadTexture("img/backgroundtemp.png");
    m_camera.zoom = 1.0f;
    m_camTarget = m_player.getPosition();

    loadLevel(1);
}

void Game::loadLevel(int t_level)
{
    std::string name;
    std::string filename = "level" + std::to_string(t_level) + ".json";
    std::string debug = "levelclear.json";

    std::ifstream file(filename);
    json data = json::parse(file);

    float size = data["walls"][0]["size"];

    for (int i = 1; i < data["walls"][0].size(); i++)
    {
        name = "wall" + std::to_string(i);
        m_walls.push_back(Wall(BROWN, size, size));
        m_walls.back().setPosition({ data["walls"][0][name][0], data["walls"][0][name][1] });
    }

    for (int i = 0; i < data["enemies"][0]["light"].size(); i++)
    {
        EnemyLight light(RED, 30.0f);
        light.setPosition({ data["enemies"][0]["light"][i]["position"][0], data["enemies"][0]["light"][i]["position"][1]});
        m_light.push_back(light);
    }

    for (int i = 0; i < data["enemies"][0]["heavy"].size(); i++)
    {
        EnemyHeavy heavy(RED, 45.0f);
        heavy.setPosition({ data["enemies"][0]["heavy"][i]["position"][0], data["enemies"][0]["heavy"][i]["position"][1] });
        m_heavy.push_back(heavy);
    }
}

void Game::draw()
{
    BeginMode2D(m_camera);
    DrawFPS(0, 0);
    if (m_state == MENU)
    {
        m_menu.draw();
    }
    else if (m_state == GAMEPLAY)
    {
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

        for (EnemyLight& l : m_light)
        {
            l.draw();
        }

        for (EnemyHeavy& h : m_heavy)
        {
            h.draw();
        }

        for (Wall& wall : m_walls)
        {
            wall.draw();
        }

        if (m_rewinding)
        {
            Timeline::drawTimeline();
        }

        EndMode2D();

        DrawRectangle(10, 40, 50, 320, BLACK);
        DrawRectangle(20, 50, 30, (m_player.getMomentum() / 100) * 300, LIGHTGRAY);
    }
}

void Game::update()
{
    if (m_state == MENU)
    {
        m_menu.update();
        if (m_menu.ended())
        {
            m_state = GAMEPLAY;
        }
    }
    else if (m_state == GAMEPLAY)
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
}

void Game::standardUpdate()
{
    m_player.update();

    for (EnemyLight& l : m_light)
    {
        l.setTarget(m_player.getPosition());
        l.update();
    }

    for (EnemyHeavy& h : m_heavy)
    {
        h.setTarget(m_player.getPosition());
        h.update();
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
    // Refactor later to use spacial partitioning

    for (EnemyLight& l : m_light)
    {
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), l);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), l);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), l);

        CollisionCheck::CheckCollisionAttack(l.getAttack(), m_player);
    }

    for (EnemyHeavy& h : m_heavy)
    {
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(LIGHT), h);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(HEAVY), h);
        CollisionCheck::CheckCollisionAttack(m_player.getAttack(SPECIAL), h);

        CollisionCheck::CheckCollisionAttack(h.getAttack(), m_player);
    }
    
    for (Wall& wall : m_walls)
    {
        CollisionCheck::CheckCollisionsWall(m_player, wall);

        for (EnemyLight& l : m_light)
        {
            CollisionCheck::CheckCollisionsWall(l, wall);
        }

        for (EnemyHeavy& h : m_heavy)
        {
            CollisionCheck::CheckCollisionsWall(h, wall);
        }
    }
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
