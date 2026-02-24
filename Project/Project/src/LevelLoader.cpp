#include "LevelLoader.h"

static int m_level = 1;
static int m_progress = 0;
static int m_progresLimit = 1;
static bool m_nextLevelReady = false;
static bool m_endOfLevels = false;
static int m_wallCount = 0;

void LevelLoader::LoadLevel(std::vector<Wall>& t_w, std::vector<Goal>& t_g, std::vector<NPC>& t_e, std::vector<EnemySupport>& t_es, std::vector<Door>& t_d, Player& t_p)
{
    m_nextLevelReady = false;

    t_e.reserve(25);

    std::string name;
    std::string filename = "level" + std::to_string(m_level) + ".json";
    std::string debug = "leveltest.json";

    m_level = 0;

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

    for (int i = m_progress; i < data["goals"].size(); i++)
    {
        Goal goal(GREEN, data["goals"][i]["size"][0], data["goals"][i]["size"][1]);
        goal.setPosition({ data["goals"][i]["position"][0], data["goals"][i]["position"][1] });
        t_g.push_back(goal);
    }
    
    m_progresLimit = data["goals"].size();

    for (int room = m_progress + 1; room < data["rooms"].size() + 1; room++)
    {
        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["doors"].size(); i++)
        {
            Door door(BLUE, data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][2], data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][3]);
            door.setPosition({ data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][0], data["rooms"][room - 1][std::to_string(room)]["doors"][0]["position"][1] });
            t_d.push_back(door);
        }

        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"].size(); i++)
        {
            EnemyLight light(RED, 30.0f);
            light.setPosition({ data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"][i]["position"][0], data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"][i]["position"][1] });
            t_e.push_back(light);
            t_d.at(room - 1 - m_progress).addEnemy(&t_e.back());
        }

        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"].size(); i++)
        {
            EnemyHeavy heavy(RED, 45.0f);
            heavy.setPosition({ data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"][i]["position"][0], data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"][i]["position"][1] });
            t_e.push_back(heavy);
            t_d.at(room - 1 - m_progress).addEnemy(&t_e.back());
        }

        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["support"].size(); i++)
        {
            EnemySupport support;
            support.setPosition({ data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["support"][i]["position"][0], data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["support"][i]["position"][1] });
            t_es.push_back(support);
            t_d.at(room - 1 - m_progress).addEnemy(&t_es.back());
        }
    }

    if (m_progress == 0)
    {
        t_p.setPosition({ data["player"]["position"][0], data["player"]["position"][1] });
    }
    else
    {
        t_p.setPosition({ data["goals"][m_progress - 1]["position"][0] + 50.0f, data["goals"][m_progress - 1]["position"][1] + 50.0f });
    }
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

    saveFile();
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

void LevelLoader::saveFile()
{
    std::ofstream Save("savefile.txt");

    Save << m_level;
    Save << m_progress;
}

void LevelLoader::loadFile()
{
    std::ifstream Load;
    Load.open("savefile.txt");

    m_level = Load.get() - '0';
    m_progress = Load.get() - '0';
}

void LevelLoader::clearFile()
{
    std::ofstream Save("savefile.txt");

    Save << 1;
    Save << 0;
}

void LevelLoader::placeWall(Vector2 t_position, int t_sizeX, int t_sizeY, std::vector<Wall>& t_w)
{
    std::string filename = "leveltest.json";

    std::ifstream file(filename);

    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_wallCount = data["walls"][0].size();
    m_wallCount++;
    data["walls"][0]["wall" + std::to_string(m_wallCount)] = {(int)t_position.x, (int)t_position.y, t_sizeX, t_sizeY};

    t_w.push_back(Wall(BROWN, t_sizeX, t_sizeY));
    t_w.back().setPosition(t_position);

    file.close();

    std::ofstream write(filename);

    write << data.dump(4);

    write.close();
}
