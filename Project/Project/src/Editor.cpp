#include "Editor.h"
static int m_wallCount = 0;

void Editor::ChangeState(EditState t_choice)
{
    m_state = t_choice;
}

EditState Editor::getState()
{
    return m_state;
}

void Editor::calculateMouse(Vector2 t_mouse)
{
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

void Editor::placeObject(std::vector<GameObject>& t_go)
{
    switch (m_state)
    {
    case WALL:
        placeWall(t_go);
        break;
    case LIGHT:
        break;
    case HEAVY:
        break;
    case SUPPORT:
        break;
    case GOAL:
        break;
    case DOOR:
        break;
    case END:
        break;
    default:
        break;
    }
}

void Editor::placeWall(std::vector<GameObject>& t_w)
{
    std::string filename = "level2.json";

    std::ifstream file(filename);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_wallCount = data["walls"][0].size();
    m_wallCount++;
    data["walls"][0]["wall" + std::to_string(m_wallCount)] = { (int)m_placePos.x, (int)m_placePos.y, m_placeSize.x, m_placeSize.y };

    Wall newWall(BROWN, m_placeSize.x, m_placeSize.y);
    newWall.setPosition(m_placePos);

    t_w.push_back(newWall);

    file.close();

    std::ofstream write(filename);

    write << data.dump(4);

    write.close();
}
