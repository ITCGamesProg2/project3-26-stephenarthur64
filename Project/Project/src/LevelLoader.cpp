#include "LevelLoader.h"

static int m_level = 2;
static int m_progress = 0;
static int m_progresLimit = 1;
static bool m_nextLevelReady = false;
static bool m_endOfLevels = false;

void LevelLoader::LoadLevel(std::vector<Wall>& t_w, std::vector<Goal>& t_g, std::vector<EnemyLight>& t_l, std::vector<EnemyHeavy>& t_h, Player& t_p)
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

    float size = data["walls"][0]["size"];

    for (int i = 1; i < data["walls"][0].size(); i++)
    {
        name = "wall" + std::to_string(i);
        t_w.push_back(Wall(BROWN, size, size));
        t_w.back().setPosition({ data["walls"][0][name][0], data["walls"][0][name][1] });
    }

    Goal goal(GREEN, 100.0f, 100.0f);
    goal.setPosition({ 200, 200 });
    t_g.push_back(goal);

    for (int i = 0; i < data["enemies"][0]["light"].size(); i++)
    {
        EnemyLight light(RED, 30.0f);
        light.setPosition({ data["enemies"][0]["light"][i]["position"][0], data["enemies"][0]["light"][i]["position"][1] });
        light.setActive(data["enemies"][0]["light"][i]["active"]);
        t_l.push_back(light);
    }

    for (int i = 0; i < data["enemies"][0]["heavy"].size(); i++)
    {
        EnemyHeavy heavy(RED, 45.0f);
        heavy.setPosition({ data["enemies"][0]["heavy"][i]["position"][0], data["enemies"][0]["heavy"][i]["position"][1] });
        heavy.setActive(data["enemies"][0]["heavy"][i]["active"]);
        t_h.push_back(heavy);
    }

    t_p.setPosition({ data["player"]["position"][0], data["player"]["position"][0] });
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
    m_level = 0;
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
