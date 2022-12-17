#pragma once

#include "iwindow.h"
#include "snakegame.h"

#include <list>
#include <string>

class RecordsWindow : public IWindow<RecordsWindow>
{
public:
    struct Record
    {
        std::wstring name;
        int score;
        SnakeGame::Difficulty difficulty;
        int mapIndex;
    };

    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void addRecord(const Record& record);

    void addRecord(const std::wstring& name, int score, SnakeGame::Difficulty difficulty, int mapIndex)
    {
        addRecord(Record{name, score, difficulty, mapIndex});
    }

    void setFileName(const std::string& fileName);

    std::string fileName() { return m_fileName; }

private:
    std::string m_fileName = "records.json";

    std::list<Record> m_records;
};
