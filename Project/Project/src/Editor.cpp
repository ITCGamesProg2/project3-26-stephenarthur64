#include "Editor.h"

Editor::Editor() : m_entityCount(0), m_room(1), m_placePos({INFINITY, 1.0f}), m_currentState(0), m_currentLevel("levels/leveledit.json"), m_uiInteract(false), m_debug(false), 
                    m_goalCount(0), m_endDFS(false)
{
    EditState m_allStates[END] = { SELECT, WALL, LIGHTENEMY, HEAVYENEMY, SUPPORTENEMY, GOAL, DOOR };

    m_save.setSize({ 100.0f, 50.0f });
    m_save.setPosition({ SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - 100.0f });
    m_save.setText("Save");

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].setSize({ 100.0f, 50.0f });
        m_objectButtons[i].setPosition({SCREEN_WIDTH - 100.0f, 100.0f + (75 * i)});
    }

    m_objectButtons[EditState::SELECT].setText("Select");
    m_objectButtons[EditState::WALL].setText("Wall");
    m_objectButtons[EditState::DOOR].setText("Door");
    m_objectButtons[EditState::LIGHTENEMY].setText("Light");
    m_objectButtons[EditState::HEAVYENEMY].setText("Heavy");
    m_objectButtons[EditState::SUPPORTENEMY].setText("Support");
    m_objectButtons[EditState::GOAL].setText("Goal");

    m_resume.setSize({ 100.0f, 50.0f });
    m_resume.setPosition({ 100.0f, SCREEN_HEIGHT - 100.0f });
    m_resume.setText("Resume");

    m_undo.setSize({ 100.0f, 50.0f });
    m_undo.setPosition({ 100.0f, 100.0f });
    m_undo.setText("Undo");
}

void Editor::drawUI()
{
    m_save.draw();

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].draw();
    }

    m_resume.draw();
    m_undo.draw();
}

void Editor::drawDebug()
{
    if (m_debug)
    {
        for (int x = 0; x < 50; x++)
        {
            for (int y = 0; y < 50; y++)
            {
                if (LevelLoader::getGridData(x, y)->getType() == CellType::WALL)
                {
                    DrawRectangle(x * 100, y * 100, 100, 100, { 160,82,45, 100 });
                }
                else if (LevelLoader::getGridData(x, y)->getType() == CellType::GOAL)
                {
                    DrawRectangle(x * 100, y * 100, 100, 100, GREEN);
                }

                if (LevelLoader::getGridData(x, y)->visited())
                {
                    DrawRectangle(x * 100.0f, y * 100.0f, 100, 100 ,{ 0, 0, 255, 100 });
                }
                DrawRectangleLinesEx({ x * 100.0f, y * 100.0f, 100, 100 }, 1.0f, { 255, 255, 255, 100 });

                DrawText((std::to_string(LevelLoader::getGridData(x, y)->getX()) + " " + std::to_string(LevelLoader::getGridData(x, y)->getY())).c_str(), (x * 100.0f) + 5.0f, (y * 100.0f) + 5.0f, 7.0f, WHITE);
            }
        }
    }
}

void Editor::handleInputs(bool& t_placing, Camera2D& t_cam)
{
    m_uiInteract = false;

    if (IsKeyReleased(KEY_F1))
    {
        m_debug = !m_debug;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
    {
        m_spawnPos = GetScreenToWorld2D(GetMousePosition(), t_cam);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        t_placing = true;
        m_mousePos = GetScreenToWorld2D(GetMousePosition(), t_cam);

        calculateMouse(m_mousePos);
    }

    m_save.detectHover(GetMousePosition());
    m_undo.detectHover(GetMousePosition());

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].detectHover(GetMousePosition());
    }

    m_resume.detectHover(GetMousePosition());

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        checkDoorEnemyClick(GetScreenToWorld2D(GetMousePosition(), t_cam));

        m_save.detectClick(GetMousePosition());
        m_undo.detectClick(GetMousePosition());

        m_resume.detectClick(GetMousePosition());

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
                for (Button& b : m_objectButtons)
                {
                    b.fullDeselect();
                }
                m_objectButtons[i].fullSelect();
                m_state = static_cast<EditState>(i);
                m_uiInteract = true;
                m_objectButtons[i].resetTrigger();
            }
        }

        t_placing = false;

        if (!m_uiInteract && !m_resume.triggered() && !m_undo.triggered())
        {
            placeObject();
        }
        m_placePos.x = INFINITY;

        if (m_undo.triggered())
        {
            undo();
            m_undo.resetTrigger();
        }
    }

    for (int i = 0; i < EditState::END; i++)
    {
        m_objectButtons[i].draw();
    }
}

void Editor::checkDoorEnemyClick(Vector2 t_mouse)
{
    for (NPC& e : *m_enemies)
    {
        if (CheckCollisionPointCircle(t_mouse, e.getPosition(), e.getRadius()))
        {
            if (m_selectedEnemy != nullptr)
            {
                m_selectedEnemy->deselect();
            }
            m_selectedEnemy = &e;
            m_selectedEnemy->select();
        }
    }

    for (NPC& es : *m_es)
    {
        if (CheckCollisionPointCircle(t_mouse, es.getPosition(), es.getRadius()))
        {
            if (m_selectedEnemy != nullptr)
            {
                m_selectedEnemy->deselect();
            }
            m_selectedEnemy = &es;
            m_selectedEnemy->select();
        }
    }

    for (int room = 0; room < m_doors->size(); room++)
    {
        if (CheckCollisionPointRec(t_mouse, m_doors->at(room).GetHitbox()))
        {
            if (m_selectedEnemy != nullptr)
            {
                m_doors->at(room).addEnemy(m_selectedEnemy);
                m_selectedEnemy->deselect();
            }
        }
    }
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
            m_placeSize.x -= (int)m_placeSize.x % 100;
        }
        if ((int)m_placeSize.y % 100 != 0)
        {
            m_placeSize.y -= (int)m_placeSize.y % 100;
        }

        if (m_placeSize.x < 100 && m_placeSize.x > -100)
        {
            m_placeSize.x = 100;
        }
        if (m_placeSize.y < 100 && m_placeSize.y > -100)
        {
            m_placeSize.y = 100;
        }

        if (m_placeSize.x < 0)
        {
            int thing = 0;
        }
    }
}

void Editor::placeObject()
{
    if (m_placePos.x < 0 || m_placePos.y < 0 || m_placePos.x > 5000 || m_placePos.y > 5000 || m_placePos.x + m_placeSize.x > 5000 || m_placePos.y + m_placeSize.y > 5000)
    {
        return;
    }

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
    if (m_placeSize.x <= -100)
    {
        m_placePos.x += m_placeSize.x;
        m_placeSize.x *= -1;
    }
    if (m_placeSize.y <= -100)
    {
        m_placePos.y += m_placeSize.y;
        m_placeSize.y *= -1;
    }
    m_placePos.x = (int)m_placePos.x;
    m_placePos.y = (int)m_placePos.y;

    m_placeSize.x = (int)m_placeSize.x;
    m_placeSize.y = (int)m_placeSize.y;

    LevelLoader::setGridData(m_placePos.x, m_placePos.y, m_placeSize.x, m_placeSize.y, CellType::WALL);

    if (checkPlacing(m_placePos, m_placeSize.x, m_placeSize.y))
    {
        m_actionList.push_back(m_state);

        Wall newWall(BROWN, m_placeSize.x, m_placeSize.y);
        newWall.setPosition(m_placePos);
        m_walls->push_back(newWall);
    }

}

void Editor::placeDoor()
{
    if (m_placeSize.x <= -100)
    {
        m_placePos.x += m_placeSize.x;
        m_placeSize.x *= -1;
    }
    if (m_placeSize.y <= -100)
    {
        m_placePos.y += m_placeSize.y;
        m_placeSize.y *= -1;
    }
    m_placePos.x = (int)m_placePos.x;
    m_placePos.y = (int)m_placePos.y;

    m_placeSize.x = (int)m_placeSize.x;
    m_placeSize.y = (int)m_placeSize.y;

    if (checkPlacing(m_placePos, m_placeSize.x, m_placeSize.y))
    {
        m_actionList.push_back(m_state);

        Door newDoor(BLUE, m_placeSize.x, m_placeSize.y);
        newDoor.setPosition(m_placePos);
        m_doors->push_back(newDoor);
    }
}

void Editor::placeGoal()
{
    if (m_placeSize.x <= -100)
    {
        m_placePos.x += m_placeSize.x;
        m_placeSize.x *= -1;
    }
    if (m_placeSize.y <= -100)
    {
        m_placePos.y += m_placeSize.y;
        m_placeSize.y *= -1;
    }
    m_placePos.x = (int)m_placePos.x;
    m_placePos.y = (int)m_placePos.y;

    m_placeSize.x = (int)m_placeSize.x;
    m_placeSize.y = (int)m_placeSize.y;

    LevelLoader::setGridData(m_placePos.x, m_placePos.y, m_placeSize.x, m_placeSize.y, CellType::GOAL);

    Goal newGoal(GREEN, m_placeSize.x, m_placeSize.y);

    if(checkPlacing(m_placePos, m_placeSize.x, m_placeSize.y))
    {
        m_actionList.push_back(m_state);

        newGoal.setPosition(m_placePos);

        m_goals->push_back(newGoal);
    }
}

void Editor::placeLight()
{
    EnemyLight newLight;

    if (checkPlacing(m_mousePos, newLight.getRadius()))
    {
        m_actionList.push_back(m_state);

        newLight.setPosition(m_mousePos);
        newLight.setSprite(&AssetManager::getSprite("light"));

        m_enemies->push_back(newLight);
    }
}

void Editor::placeHeavy()
{
    EnemyHeavy newHeavy;

    if (checkPlacing(m_mousePos, newHeavy.getRadius()))
    {
        m_actionList.push_back(m_state);

        newHeavy.setPosition(m_mousePos);
        newHeavy.setSprite(&AssetManager::getSprite("heavy"));

        m_enemies->push_back(newHeavy);
    }
}

void Editor::placeSupport()
{
    EnemySupport newSupport;

    if (checkPlacing(m_mousePos, newSupport.getRadius()))
    {
        m_actionList.push_back(m_state);

        newSupport.setPosition(m_mousePos);
        newSupport.setSprite(&AssetManager::getSprite("support"));

        m_es->push_back(newSupport);
    }
}

void Editor::writeObjectsToFile()
{
    std::ifstream file(m_currentLevel);
    data = nlohmann::json::parse(file);

    data["player"]["position"][0] = (int)m_spawnPos.x;
    data["player"]["position"][1] = (int)m_spawnPos.y;

    for (int index = 0; index < m_walls->size(); index++)
    {
        data["walls"][0]["wall" + std::to_string(index + 1)] = { (int)m_walls->at(index).GetHitbox().x, (int)m_walls->at(index).GetHitbox().y, (int)m_walls->at(index).GetHitbox().width, (int)m_walls->at(index).GetHitbox().height };
    }

    for (int room = 0; room < m_doors->size(); room++)
    {
        data["rooms"][room][std::to_string(room + 1)]["doors"][0]["position"] = { (int)m_doors->at(room).GetHitbox().x, (int)m_doors->at(room).GetHitbox().y, (int)m_doors->at(room).GetHitbox().width, (int)m_doors->at(room).GetHitbox().height };

        for (int e = 0; e < m_doors->at(room).getEnemies().size(); e++)
        {
            switch (m_doors->at(room).getEnemies().at(e)->getType())
            {
            case LIGHT:
                data["rooms"][room][std::to_string(room + 1)]["enemies"][0]["light"][e]["position"] = { (int)m_doors->at(room).getEnemies().at(e)->getPosition().x, (int)m_doors->at(room).getEnemies().at(e)->getPosition().y };
                break;
            case HEAVY:
                data["rooms"][room][std::to_string(room + 1)]["enemies"][0]["heavy"][e]["position"] = { (int)m_doors->at(room).getEnemies().at(e)->getPosition().x, (int)m_doors->at(room).getEnemies().at(e)->getPosition().y };
                break;
            case SUPPORT:
                data["rooms"][room][std::to_string(room + 1)]["enemies"][0]["support"][e]["position"] = { (int)m_doors->at(room).getEnemies().at(e)->getPosition().x, (int)m_doors->at(room).getEnemies().at(e)->getPosition().y };
                break;
            default:
                break;
            }
        }
    }

    for (int index = 0; index < m_goals->size(); index++)
    {
        data["goals"][index]["position"] = { (int)m_goals->at(index).getPosition().x, (int)m_goals->at(index).getPosition().y };
        data["goals"][index]["size"] = { (int)m_goals->at(index).GetHitbox().width, (int)m_goals->at(index).GetHitbox().height };
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
    initDFS();

    if (m_endDFS)
    {
        writeObjectsToFile();

        std::ofstream write("levels/leveledit.json");

        write << data.dump(4);

        write.close();
    }
    else
    {
        std::cout << "DFS failed\n";
    }
}

void Editor::undo()
{
    if (m_actionList.size() > 0)
    {
        switch (m_actionList.back())
        {
        case WALL:
            LevelLoader::setGridData(m_walls->back().GetHitbox().x, m_walls->back().GetHitbox().y, m_walls->back().GetHitbox().width, m_walls->back().GetHitbox().height, CellType::EMPTY);
            m_walls->pop_back();
            break;
        case GOAL:
            LevelLoader::setGridData(m_goals->back().GetHitbox().x, m_goals->back().GetHitbox().y, m_goals->back().GetHitbox().width, m_goals->back().GetHitbox().height, CellType::EMPTY);
            m_goals->pop_back();
            break;
        case DOOR:
            m_doors->pop_back();
            break;
        case LIGHTENEMY:
        case HEAVYENEMY:
            m_enemies->pop_back();
            break;
        case SUPPORTENEMY:
            m_es->pop_back();
            break;
        default:
            break;
        }

        m_actionList.pop_back();
    }
}

void Editor::initDFS()
{
    Vector2 start = { (int)(m_spawnPos.x / 100), (int)(m_spawnPos.y / 100) };
    m_endDFS = false;

    while (m_stackDFS.size() > 0)
    {
        m_stackDFS.pop();
    }

    for (int x = 0; x < 50; x++)
    {
        for (int y = 0; y < 50; y++)
        {
            LevelLoader::getGridData(x, y)->unvisit();
        }
    }

    m_stackDFS.push(LevelLoader::getGridData(start.x, start.y));

    DFSForComplete();
}

void Editor::DFSForComplete()
{
    while (m_stackDFS.size() > 0 && !m_endDFS)
    {
        Cell* current = m_stackDFS.top();
        m_stackDFS.pop();
        current->visit();

        for (Cell* arc : current->getArcList())
        {
            if (!arc->visited())
            {
                if (arc->getType() == CellType::GOAL)
                {
                    m_goalCount++;
                    arc->visitAllGoalNeighbours();

                    if (m_goalCount == m_goals->size())
                    {
                        m_endDFS = true;
                    }
                }
                if (arc->getType() != CellType::WALL)
                {
                    m_stackDFS.push(arc);
                    DFSForComplete();
                }
            }
        }
    }
}

