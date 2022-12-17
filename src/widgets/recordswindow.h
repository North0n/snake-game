#pragma once

#include "entities/gametypes.h"
#include "iwindow.h"
#include "services/gdipluspainter.h"
#include "snakegame.h"

#include <list>
#include <optional>
#include <string>

class RecordsWindow : public IWindow<RecordsWindow>
{
public:
    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    HWND difficultyComboBox() const { return m_difficultyComboBox; }

    HWND mapComboBox() const { return m_mapComboBox; }

    void addRecord(const Record& record);

    void addRecord(const std::wstring& name, int score, Difficulty difficulty, int mapIndex)
    {
        addRecord(Record{name, score, difficulty, mapIndex});
    }

    void clearRecords();

    void setFileName(const std::string& fileName);

    std::string fileName() { return m_fileName; }

private:
    static constexpr inline int MainMenuButtonId     = 1;
    static constexpr inline int DifficultyComboBoxId = 2;
    static constexpr inline int MapComboBoxId        = 3;
    static constexpr inline int ClearButtonId        = 4;

    static constexpr inline int RecordsOnPage = 8;

    static void setWindowRegion(HWND hwnd);

    HWND m_mainMenuButton;
    HWND m_difficultyComboBox;
    HWND m_mapComboBox;
    HWND m_clearButton;

    std::string m_fileName = "records.json";
    std::list<Record> m_records;

    std::optional<Difficulty> m_filterDifficulty{};
    std::optional<int> m_filterMapIndex{};

    std::unique_ptr<GdiPlusPainter> m_painter;
};
