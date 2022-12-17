#pragma once

#include "json.hpp"
#include "widgets/snakegame.h"

#include <fstream>
#include <string>

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
        std::ofstream file(m_fileName);          \
        file << m_json;                          \
    }

class Settings
{
    PROPERTY(int, mapIndex)
    PROPERTY(SnakeGame::Difficulty, difficulty)
    PROPERTY(std::string, playerName)

public:
    explicit Settings(const std::string& fileName);

private:
    nlohmann::json m_json;
    std::string m_fileName;
};
