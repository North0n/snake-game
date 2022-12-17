#include "settings.h"

#include <codecvt>
#include <filesystem>
#include <locale>

Settings::Settings()
{
    if (std::filesystem::exists(s_fileName)) {
        std::ifstream file(s_fileName);
        file >> m_json;
    } else {
        m_json = nlohmann::json::object();

        set_mapIndex(0);
        set_difficulty(SnakeGame::Difficulty::Easy);
        set_playerName(toUtf8(L"Игрок"));
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

std::string Settings::toUtf8(const std::wstring& wstring)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(wstring);
}

std::wstring Settings::fromUtf8(const std::string& string)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.from_bytes(string);
}
