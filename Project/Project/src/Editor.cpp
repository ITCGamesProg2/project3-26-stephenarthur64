#include "Editor.h"

Editor::Editor() : m_entityCount(0), m_room(1), m_placePos({INFINITY, 1.0f}), m_currentState(0), m_currentLevel("levels/levelclear.json"), m_uiInteract(false)
{
    EditState m_allStates[END] = { WALL, LIGHTENEMY, HEAVYENEMY, SUPPORTENEMY, GOAL, DOOR };

    m_save.setSize({ 100.0f, 50.0f });
    m_save.setPosition({ SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - 100.0f });
    m_save.setText("Save");

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].setSize({ 100.0f, 50.0f });
        m_objectButtons[i].setPosition({SCREEN_WIDTH - 200.0f, 100.0f + (75 * i)});
    }

    m_objectButtons[EditState::WALL].setText("Wall");
    m_objectButtons[EditState::DOOR].setText("Door");
    m_objectButtons[EditState::LIGHTENEMY].setText("Light");
    m_objectButtons[EditState::HEAVYENEMY].setText("Heavy");
    m_objectButtons[EditState::SUPPORTENEMY].setText("Support");
    m_objectButtons[EditState::GOAL].setText("Goal");
}

void Editor::drawUI()
{
    m_save.draw();

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].draw();
    }
}

void Editor::handleInputs(bool& t_placing, Camera2D& t_cam)
{
    m_uiInteract = false;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        t_placing = true;
        m_mousePos = GetScreenToWorld2D(GetMousePosition(), t_cam);

        calculateMouse(m_mousePos);
    }

    m_save.detectHover(GetMousePosition());

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].detectHover(GetMousePosition());
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        m_save.detectClick(GetMousePosition());

        if (m_save.triggered())
        {
            saveFile();
            m_uiInteract = true;
            m_save.resetTrigger();
        }

        for (int i = 0; i < EditState::END; i++)
        {
            m_objectButtons[i].detectClick(GetMousePosition());
            if (m_objectButtons[i].triggered())
            {
                m_state = static_cast<EditState>(i);
                m_uiInteract = true;
                m_objectButtons[i].resetTrigger();
            }
        }

        t_placing = false;

        if (!m_uiInteract)
        {
            placeObject();
        }
    }

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].draw();
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

    data = nlohmann::json::parse(file);
    Wall newWall(BROWN, m_placeSize.x, m_placeSize.y);

    if (checkPlacing(m_placePos, m_placeSize.x, m_placeSize.y))
    {
        m_entityCount = data["walls"][0].size();
        m_entityCount++;
        data["walls"][0]["wall" + std::to_string(m_entityCount)] = { (int)m_placePos.x, (int)m_placePos.y, (int)m_placeSize.x, (int)m_placeSize.y };

        newWall.setPosition(m_placePos);

        m_walls->push_back(newWall);
    }

    file.close();
}

void Editor::placeDoor()
{
    std::ifstream file(m_currentLevel);

    data = nlohmann::json::parse(file);

    Door newDoor(BLUE, m_placeSize.x, m_placeSize.y);

    if (checkPlacing(m_placePos, m_placeSize.x, m_placeSize.y))
    {
        m_entityCount = data["rooms"].size();
        m_entityCount++;

        if (m_room < m_entityCount)
        {
            m_entityCount = m_room;
        }

        data["rooms"][m_entityCount - 1][std::to_string(m_entityCount)]["doors"][0]["position"] = { (int)m_placePos.x, (int)m_placePos.y, (int)m_placeSize.x, (int)m_placeSize.y };

        newDoor.setPosition(m_placePos);

        m_doors->push_back(newDoor);
    }

    file.close();
}

void Editor::placeGoal()
{
    std::ifstream file(m_currentLevel);

    data = nlohmann::json::parse(file);

    Goal newGoal(GREEN, m_placeSize.x, m_placeSize.y);

    if(checkPlacing(m_placePos, m_placeSize.x, m_placeSize.y))
    {
        m_entityCount = data["goals"].size();

        data["goals"][m_entityCount]["position"] = { (int)m_placePos.x, (int)m_placePos.y };
        data["goals"][m_entityCount]["size"] = { (int)m_placeSize.x, (int)m_placeSize.y };

        newGoal.setPosition(m_placePos);

        m_goals->push_back(newGoal);
    }

    file.close();
}

void Editor::placeLight()
{
    std::ifstream file(m_currentLevel);

    data = nlohmann::json::parse(file);

    EnemyLight newLight;

    if (checkPlacing(m_mousePos, newLight.getRadius()))
    {
        m_entityCount = data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["light"].size();
        m_entityCount++;

        data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["light"][m_entityCount - 1]["position"] = { (int)m_mousePos.x, (int)m_mousePos.y };

        newLight.setPosition(m_mousePos);
        newLight.setSprite(&AssetManager::getSprite("light"));

        m_enemies->push_back(newLight);
    }

    file.close();
}

void Editor::placeHeavy()
{
    std::ifstream file(m_currentLevel);

    data = nlohmann::json::parse(file);

    EnemyHeavy newHeavy;

    if (checkPlacing(m_mousePos, newHeavy.getRadius()))
    {
        m_entityCount = data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["heavy"].size();
        m_entityCount++;

        data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["heavy"][m_entityCount - 1]["position"] = { (int)m_mousePos.x, (int)m_mousePos.y };

        newHeavy.setPosition(m_mousePos);
        newHeavy.setSprite(&AssetManager::getSprite("heavy"));

        m_enemies->push_back(newHeavy);
    }

    file.close();
}

void Editor::placeSupport()
{
    std::ifstream file(m_currentLevel);

    data = nlohmann::json::parse(file);

    EnemySupport newSupport;

    if (checkPlacing(m_mousePos, newSupport.getRadius()))
    {
        m_entityCount = data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["support"].size();
        m_entityCount++;

        data["rooms"][m_room - 1][std::to_string(m_room)]["enemies"][0]["support"][m_entityCount - 1]["position"] = { (int)m_mousePos.x, (int)m_mousePos.y };

        newSupport.setPosition(m_mousePos);
        newSupport.setSprite(&AssetManager::getSprite("support"));

        m_es->push_back(newSupport);
    }

    file.close();
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

bool Editor::checkPlacing(Vector2 t_pos, float t_radius)
{
    for (Wall& wall : *m_walls)
    {
        if (CheckCollisionCircleRec(t_pos, t_radius, wall.GetHitbox()))
        {
            return false;
        }
    }

    return true;
}

bool Editor::checkPlacing(Vector2 t_pos, float t_x, float t_y)
{
    for (Wall& wall : *m_walls)
    {
        if (CheckCollisionRecs(wall.GetHitbox(), { t_pos.x, t_pos.y, t_x, t_y }))
        {
            return false;
        }
    }

    return true;
}

void Editor::saveFile()
{
    std::ofstream write("levels/leveledit");

    write << data.dump(4);

    write.close();
}

