#pragma once

#include "iwindow.h"
#include "snakegame.h"

class ChangeModeWidget : public IWindow<ChangeModeWidget>
{
public:
    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void setDifficulty(SnakeGame::Difficulty difficulty) { m_difficulty = difficulty; }

    [[nodiscard]] SnakeGame::Difficulty difficulty() const { return m_difficulty; }

private:
    static constexpr inline int EasyButtonId   = 1;
    static constexpr inline int MediumButtonId = 2;
    static constexpr inline int HardButtonId   = 3;

    static constexpr inline int MainMenuButtonId = 4;

    static void setWindowRegion(HWND hwnd);

    HWND m_easyButton;
    HWND m_mediumButton;
    HWND m_hardButton;

    HWND m_mainMenuButton;

    SnakeGame::Difficulty m_difficulty;
};
