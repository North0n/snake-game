#include "settings.h"

#include <filesystem>

Settings::Settings()
{
    if (std::filesystem::exists(s_fileName))
    {
        std::ifstream file(s_fileName);
        file >> m_json;
    }
    else
    {
        m_json = nlohmann::json::object();

        set_mapIndex(0);
        set_difficulty(SnakeGame::Difficulty::Easy);
        set_playerName("Player");
    }
}

void Settings::setFileName(const std::string& fileName)
{
    s_fileName = fileName;
}

std::string Settings::fileName()
{
    return s_fileName;
}