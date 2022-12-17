#pragma once

#include <string>

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

struct Record
{
    std::wstring name;
    int score;
    Difficulty difficulty;
    int mapIndex;
};
