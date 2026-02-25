#include "Editor.h"
static int m_entityCount = 0;
static int m_room = 1;
static EditState m_state;
static Vector2 m_placePos = {INFINITY, 1.0f};
static Vector2 m_placeSize;
static Vector2 m_mousePos;
static std::vector<Wall>* m_walls;
static std::vector<NPC>* m_enemies;
static std::vector<EnemySupport>* m_es;
static std::vector<Door>* m_doors;
static std::vector<Goal>* m_goals;
static int m_currentState = 0;
static EditState m_allStates[END] = { WALL, LIGHTENEMY, HEAVYENEMY, SUPPORTENEMY, GOAL, DOOR };
static std::string m_currentLevel = "leveltest.json";

void Editor::handleInputs(bool& t_placing, Camera2D& t_cam)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        t_placing = true;
        m_mousePos = GetScreenToWorld2D(GetMousePosition(), t_cam);

        calculateMouse(m_mousePos);
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        t_placing = false;

        placeObject();
    }

    if (IsKeyReleased(KEY_F1))
    {
        changeRoom(1);
    }
    if (IsKeyReleased(KEY_F2))
    {
        changeRoom(-1);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
    {
        cycleStates();
    }
}

void Editor::cycleStates()
{
    if (m_currentState + 1 < END)
    {
        m_currentState++;
    }
    else
    {
        m_currentState = 0;
    }

    m_state = m_allStates[m_currentState];
}

void Editor::changeRoom(int t_direction)
{
    m_room += t_direction;
}

int Editor::getRoom()
{
    return m_room;
}

std::string Editor::getState()
{
    switch (m_state)
    {
    case WALL:
        return "Wall";
    case LIGHTENEMY:
        return "Light Enemy";
    case HEAVYENEMY:
        return "Heavy Enemy";
    case SUPPORTENEMY:
        return "Support Enemy";
    case GOAL:
        return "Goal";
    case DOOR:
        return "Door";
    default:
        break;
    }

    return std::string();
}



void Editor::calculateMouse(Vector2 t_mouse)
{
    m_mousePos = t_mouse;

    if (m_placePos.x == INFINITY)
    {
        m_placePos = m_mousePos;
        m_placePos.x = m_placePos.x - ((int)m_placePos.x % 100);
        m_placePos.y = m_placePos.y - ((int)m_placePos.y % 100);
    }
    else
    {
        m_placeSize.x = (m_mousePos.x + (int)m_mousePos.x % 100) - m_placePos.x;
        m_placeSize.y = (m_mousePos.y + (int)m_mousePos.y % 100) - m_placePos.y;

        if ((int)m_placeSize.x % 100 != 0)
        {
            m_placeSize.x += 100 - (int)m_placeSize.x % 100;
        }
        if ((int)m_placeSize.y % 100 != 0)
        {
            m_placeSize.y += 100 - (int)m_placeSize.y % 100;
        }

        if (m_placeSize.x < 100)
        {
            m_placeSize.x = 100;
        }
        if (m_placeSize.y < 100)
        {
            m_placeSize.y = 100;
        }
    }
}

void Editor::placeObject()
{
    switch (m_state)
    {
    case WALL:
        placeWall();
        break;
    case LIGHTENEMY:
        placeLight();
        break;
    case HEAVYENEMY:
        placeHeavy();
        break;
    case SUPPORTENEMY:
        placeSupport();
        break;
    case GOAL:
        placeGoal();
        break;
    case DOOR:
        placeDoor();
        break;
    case END:
        break;
    default:
        break;
    }

    m_placePos.x = INFINITY;
}

void Editor::placeWall()
{
    std::ifstream file(m_currentLevel);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_entityCount = data["walls"][0].size();
    m_entityCount++;
    data["walls"][0]["wall" + std::to_string(m_entityCount)] = { (int)m_placePos.x, (int)m_placePos.y, m_placeSize.x, m_placeSize.y };

    Wall newWall(BROWN, m_placeSize.x, m_placeSize.y);
    newWall.setPosition(m_placePos);

    m_walls->push_back(newWall);

    file.close();

    std::ofstream write(m_currentLevel);

    write << data.dump(4);

    write.close();
}

void Editor::placeDoor()
{
    std::ifstream file(m_currentLevel);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_entityCount = data["rooms"].size();
    m_entityCount++;

    if (m_room < m_entityCount)
    {
        m_entityCount = m_room;
    }

    data["rooms"][m_entityCount - 1][std::to_string(m_entityCount)]["doors"][0]["position"] = {(int)m_placePos.x, (int)m_placePos.y, (int)m_placeSize.x, (int)m_placeSize.y};

    Door newDoor(BLUE, m_placeSize.x, m_placeSize.y);
    newDoor.setPosition(m_placePos);

    m_doors->push_back(newDoor);

    file.close();

    std::ofstream write(m_currentLevel);

    write << data.dump(4);

    write.close();
}

void Editor::placeGoal()
{
    std::ifstream file(m_currentLevel);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_entityCount = data["goals"].size();

    data["goals"][m_entityCount]["position"] = {(int)m_placePos.x, (int)m_placePos.y};
    data["goals"][m_entityCount]["size"] = { (int)m_placeSize.x, (int)m_placeSize.y };

    Goal newGoal(GREEN, m_placeSize.x, m_placeSize.y);
    newGoal.setPosition(m_placePos);

    m_goals->push_back(newGoal);

    file.close();

    std::ofstream write(m_currentLevel);

    write << data.dump(4);

    write.close();
}

void Editor::placeLight()
{
    std::ifstream file(m_currentLevel);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_entityCount = data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["light"].size();
    m_entityCount++;

    data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["light"][m_entityCount - 1]["position"] = { (int)m_mousePos.x, (int)m_mousePos.y};

    EnemyLight newLight;
    newLight.setPosition(m_mousePos);

    m_enemies->push_back(newLight);

    file.close();

    std::ofstream write(m_currentLevel);

    write << data.dump(4);

    write.close();
}

void Editor::placeHeavy()
{
    std::ifstream file(m_currentLevel);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_entityCount = data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["heavy"].size();
    m_entityCount++;

    data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["heavy"][m_entityCount - 1]["position"] = { (int)m_mousePos.x, (int)m_mousePos.y };

    EnemyHeavy newHeavy;
    newHeavy.setPosition(m_mousePos);

    m_enemies->push_back(newHeavy);

    file.close();

    std::ofstream write(m_currentLevel);

    write << data.dump(4);

    write.close();
}

void Editor::placeSupport()
{
    std::ifstream file(m_currentLevel);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_entityCount = data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["support"].size();
    m_entityCount++;

    data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["support"][m_entityCount - 1]["position"] = { (int)m_mousePos.x, (int)m_mousePos.y };

    EnemySupport newSupport;
    newSupport.setPosition(m_mousePos);

    m_es->push_back(newSupport);

    file.close();

    std::ofstream write(m_currentLevel);

    write << data.dump(4);

    write.close();
}

void Editor::setWallReference(std::vector<Wall>* t_w)
{
    m_walls = t_w;
}

void Editor::setNPCReference(std::vector<NPC>* t_e)
{
    m_enemies = t_e;
}

void Editor::setSupportReference(std::vector<EnemySupport>* t_es)
{
    m_es = t_es;
}

void Editor::setDoorReference(std::vector<Door>* t_d)
{
    m_doors = t_d;
}

void Editor::setGoalReference(std::vector<Goal>* t_g)
{
    m_goals = t_g;
}

void Editor::drawPlacing()
{
    switch (m_state)
    {
    case WALL:
    case DOOR:
    case GOAL:
        DrawRectangleLines(m_placePos.x, m_placePos.y, m_placeSize.x, m_placeSize.y, DARKBLUE);
        break;
    case LIGHTENEMY:
        break;
    case HEAVYENEMY:
        break;
    case SUPPORTENEMY:
        break;
    case END:
        break;
    default:
        break;
    }
}
