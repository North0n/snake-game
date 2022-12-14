#pragma once

#include "entities/gametypes.h"
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
        file << std::setw(4) << m_json;          \
    }

class Settings
{
    PROPERTY(int, mapIndex)
    PROPERTY(Difficulty, difficulty)
    PROPERTY(std::string, playerName)

public:
    explicit Settings();

    static std::string toUtf8(const std::wstring& wstring);

    static std::wstring fromUtf8(const std::string& string);

    static void setFileName(const std::string& fileName);

    static std::string fileName();

private:
    static inline std::string s_fileName = "settings.json";

    nlohmann::json m_json;
};
