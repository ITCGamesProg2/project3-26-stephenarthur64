#include "LevelLoader.h"

static int m_level = 1;
static int m_progress = 0;
static int m_progresLimit = 1;
static bool m_nextLevelReady = false;
static bool m_endOfLevels = false;

void LevelLoader::LoadLevel(std::vector<Wall>& t_w, std::vector<Goal>& t_g, std::vector<EnemyLight>& t_l, std::vector<EnemyHeavy>& t_h, std::vector<Door>& t_d, Player& t_p)
{
    m_nextLevelReady = false;

    std::string name;
    std::string filename = "level" + std::to_string(m_level) + ".json";
    std::string debug = "levelclear.json";

    if (m_level == 0)
    {
        filename = debug;
    }

    std::ifstream file(filename);
    nlohmann::json data;
    try
    {
        data = nlohmann::json::parse(file);
    }
    catch (...)
    {
        m_endOfLevels = true;
        return;
    }

    int size;

    for (int i = 0; i < data["walls"][0].size(); i++)
    {
        name = "wall" + std::to_string(i + 1);
        t_w.push_back(Wall(BROWN, data["walls"][0][name][2], data["walls"][0][name][3]));
        t_w.back().setPosition({ data["walls"][0][name][0], data["walls"][0][name][1] });
    }

    for (int i = 0; i < data["goals"][0].size(); i++)
    {
        Goal goal(GREEN, data["goals"][i]["size"][0], data["goals"][i]["size"][1]);
        goal.setPosition({ data["goals"][i]["position"][0], data["goals"][i]["position"][1] });
        t_g.push_back(goal);
    }
    
    m_progresLimit = data["goals"][0].size();

    for (int room = 1; room < data["rooms"].size() + 1; room++)
    {
        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["doors"].size(); i++)
        {
            Door door(BLUE, data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][2], data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][3]);
            door.setPosition({ data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][0], data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][1] });
            t_d.push_back(door);
        }

        t_l.reserve(data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"].size());

        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"].size(); i++)
        {
            EnemyLight light(RED, 30.0f);
            light.setPosition({ data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"][i]["position"][0], data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"][i]["position"][1] });
            light.setActive(data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"][i]["active"]);
            t_l.push_back(light);
            t_d.at(room - 1).addEnemy(&t_l.at(i));
        }

        t_h.reserve(data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"].size());

        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"].size(); i++)
        {
            EnemyHeavy heavy(RED, 45.0f);
            heavy.setPosition({ data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"][i]["position"][0], data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"][i]["position"][1] });
            heavy.setActive(data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"][i]["active"]);
            t_h.push_back(heavy);
        }
    }

    t_p.setPosition({ data["player"]["position"][0], data["player"]["position"][1] });
}

void LevelLoader::addProgress()
{
	m_progress++;

    if (m_progress == m_progresLimit)
    {
        m_level++;
        m_nextLevelReady = true;
        m_progress = 0;
    }
}

void LevelLoader::clearProgress()
{
    m_level = 1;
    m_progress = 0;
    m_nextLevelReady = false;
    m_endOfLevels = false;
}

bool LevelLoader::isNextLevelReady()
{
    return m_nextLevelReady;
}

bool LevelLoader::isAtEnd()
{
    return m_endOfLevels;
}
