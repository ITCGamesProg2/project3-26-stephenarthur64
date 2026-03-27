#include "LevelLoader.h"

static int m_level = 1;
static int m_progress = 0;
static int m_progresLimit = 1;
static bool m_nextLevelReady = false;
static bool m_endOfLevels = false;
static int m_currentFile = 1;
static Player* m_player;
static SaveDetails m_saves[3];
static float m_music;
static float m_sfx;

void LevelLoader::LoadLevel(std::vector<Wall>& t_w, std::vector<Goal>& t_g, std::vector<NPC>& t_e, std::vector<EnemySupport>& t_es, std::vector<Door>& t_d, TimeAbilities& t_bossType, Vector2& t_bossPos)
{
    m_nextLevelReady = false;

    t_e.reserve(25);
    t_es.reserve(25);

    std::string name;
    std::string filename = "levels/level" + std::to_string(m_level) + ".json";
    std::string debug = "levels/levelclear.json";

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
            EnemyLight light;
            light.setPosition({ data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"][i]["position"][0], data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["light"][i]["position"][1] });
            t_e.push_back(light);
            t_d.at(room - 1 - m_progress).addEnemy(&t_e.back());
        }

        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["heavy"].size(); i++)
        {
            EnemyHeavy heavy;
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
        for (int i = 0; i < data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["boss"].size(); i++)
        {
            t_bossPos = { data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["boss"][i]["position"][0], data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["boss"][i]["position"][1] };

            if (data["rooms"][room - 1][std::to_string(room)]["enemies"][0]["boss"][i]["type"] == "rewind")
            {
                t_bossType = TimeAbilities::REWIND;
            }
        }
    }

    if (m_progress == 0)
    {
        m_player->setPosition({ data["player"]["position"][0], data["player"]["position"][1] });
    }
    else
    {
        m_player->setPosition({ data["goals"][m_progress - 1]["position"][0] + 50.0f, data["goals"][m_progress - 1]["position"][1] + 50.0f });
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

    saveFile(m_currentFile);
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

void LevelLoader::saveFile(int t_file)
{
    m_currentFile = t_file + 1;

    std::ifstream file("saves/savefile" + std::to_string(m_currentFile) + ".json");
    nlohmann::json data;
    data = nlohmann::json::parse(file);

    data["health"] = m_player->getHealthPercentage();
    data["momentum"] = m_player->getMomentumPercentage();
    data["rewind"] = m_player->canUse(REWIND);
    data["skip"] = m_player->canUse(SKIP);
    data["stop"] = m_player->canUse(STOP);

    data["level"] = m_level;
    data["progress"] = m_progress;

    file.close();

    std::ofstream write("saves/savefile" + std::to_string(m_currentFile) + ".json");

    write << data.dump(4);

    write.close();
}

void LevelLoader::loadFile(int t_file)
{
    t_file--;
    m_currentFile = t_file;

    m_player->loadValues(m_saves[t_file].healthPercentage, m_saves[t_file].momentumPercentage, m_saves[t_file].rewind, m_saves[t_file].skip, m_saves[t_file].stop);

    m_level = m_saves[t_file].level;
    m_progress = m_saves[t_file].progress;
}

void LevelLoader::clearFile(int t_file)
{
    m_currentFile = t_file;

    std::ofstream Save("saves/savefile" + std::to_string(t_file) + ".txt");

    Save << 1;
    Save << 0;
}

SaveDetails LevelLoader::getSaveDetails(int t_file)
{
    return m_saves[t_file];
}

void LevelLoader::loadSaves()
{
    for (int i = 0; i < 3; i++)
    {
        std::ifstream file("saves/savefile" + std::to_string(i + 1) + ".json");
        nlohmann::json data;
        data = nlohmann::json::parse(file);

        m_saves[i].level = data["level"];
        m_saves[i].progress = data["progress"];

        m_saves[i].healthPercentage = data["health"];
        m_saves[i].momentumPercentage = data["momentum"];
        m_saves[i].rewind = data["rewind"];
        m_saves[i].skip = data["skip"];
        m_saves[i].stop = data["stop"];
    }
}

void LevelLoader::setPlayerRef(Player* t_p)
{
    m_player = t_p;
}

void LevelLoader::saveOptions(float t_music, float t_sfx)
{
    std::ifstream file("options.json");
    nlohmann::json data;
    data = nlohmann::json::parse(file);

    data["music"] = t_music;
    data["sfx"] = t_sfx;

    file.close();

    std::ofstream write("options.json");

    write << data.dump(4);

    write.close();
}

float LevelLoader::getMusicVolume()
{
    return m_music;
}

float LevelLoader::getSFXVolume()
{
    return m_sfx;
}

void LevelLoader::loadOptions()
{
    std::ifstream file("options.json");
    nlohmann::json data;
    data = nlohmann::json::parse(file);

    m_music = data["music"];
    m_sfx = data["sfx"];
}

