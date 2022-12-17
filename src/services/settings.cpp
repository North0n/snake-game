#include "settings.h"

#include <filesystem>

Settings::Settings(const std::string& fileName)
    : m_fileName(fileName)
{
    if (std::filesystem::exists(fileName))
    {
        std::ifstream file(fileName);
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
