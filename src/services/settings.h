#pragma once

#include "json.hpp"
#include "singleton.h"
#include "widgets/snakegame.h"

#include <fstream>
#include <string>

#define appSettings SINGLETON(Settings)

#define PROPERTY(type, name)                     \
public:                                          \
    type name() const                            \
    {                                            \
        return static_cast<type>(m_json[#name]); \
    }                                            \
                                                 \
public:                                          \
    void set_##name(type value)                  \
    {                                            \
        m_json[#name] = value;                   \
        std::ofstream file(s_fileName);          \
        file << m_json;                          \
    }

class Settings
{
    PROPERTY(int, mapIndex)
    PROPERTY(SnakeGame::Difficulty, difficulty)
    PROPERTY(std::string, playerName)

public:
    explicit Settings();

    static void setFileName(const std::string& fileName);

    static std::string fileName();

private:
    static inline std::string s_fileName = "settings.json";

    nlohmann::json m_json;
};
