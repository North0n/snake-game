#include "recordswindow.h"

#include "json.hpp"
#include "services/settings.h"

#include <filesystem>
#include <fstream>

PCWSTR RecordsWindow::className() const
{
    return L"Records Window Class";
}

LRESULT RecordsWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE: {
        setFileName(m_fileName);
        return 0;
    }
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void RecordsWindow::addRecord(const RecordsWindow::Record& record)
{
    auto it = std::find_if(m_records.begin(), m_records.end(), [&record](const Record& r) {
        return r.score > record.score;
    });
    m_records.insert(it, record);

    nlohmann::json json;
    std::transform(m_records.begin(), m_records.end(), std::back_inserter(json), [](const Record& r) {
        return nlohmann::json{
            {"name", Settings::toUtf8(r.name)},
            {"score", r.score},
            {"difficulty", r.difficulty},
            {"mapIndex", r.mapIndex}};
    });

    std::ofstream file(m_fileName);
    file << std::setw(4) << json;
}

void RecordsWindow::setFileName(const std::string& fileName)
{
    m_fileName = fileName;
    m_records.clear();

    if (!std::filesystem::exists(m_fileName)) {
        return;
    }

    std::ifstream file(m_fileName);
    nlohmann::json json;
    file >> json;

    for (const auto& record : json) {
        Record r;
        r.name       = Settings::fromUtf8(record["name"].get<std::string>());
        r.score      = record["score"].get<int>();
        r.difficulty = static_cast<SnakeGame::Difficulty>(record["difficulty"].get<int>());
        r.mapIndex   = record["mapIndex"].get<int>();
        m_records.push_back(r);
    }
}
