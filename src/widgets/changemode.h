#pragma once

#include "iwindow.h"
#include "snakegame.h"

class ImageContainer;

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

    static constexpr inline int Map1ButtonId = 5;
    static constexpr inline int Map2ButtonId = 6;
    static constexpr inline int Map3ButtonId = 7;

    static const inline std::vector<std::vector<Point>> MapsOfObstacles {
        {}, // Empty map
        {{160, 80}, {160, 120}, {160, 160}, {160, 200}, {160, 240}, {160, 280}, {160, 320}, {160, 360}, {160, 400}, {160, 440}, {160, 480},
         {440, 80}, {480, 80}, {520, 80}, {560, 80}, {600, 80}, {640, 80}, {680, 80}, {720, 80},
         {1000, 80}, {1000, 120}, {1000, 160}, {1000, 200}, {1000, 240}, {1000, 280}, {1000, 320}, {1000, 360}, {1000, 400}, {1000, 440}, {1000, 480},
         {440, 480}, {480, 480}, {520, 480}, {560, 480}, {600, 480}, {640, 480}, {680, 480}, {720, 480}},
        {{200, 80}, {240, 80}, {280, 80}, {320, 80}, {360, 80}, {400, 80}, {440, 80}, {480, 80}, {520, 80},
         {80, 320}, {120, 360}, {160, 400}, {200, 440}, {240, 480},
         {640, 480}, {680, 480}, {720, 480}, {760, 480}, {800, 480}, {840, 480}, {880, 480}, {920, 480}, {960, 480},
         {920, 80}, {960, 120}, {1000, 160}, {1040, 200}, {1080, 240}}
    };

    static void setWindowRegion(HWND hwnd);

    HWND m_easyButton;
    HWND m_mediumButton;
    HWND m_hardButton;

    HWND m_mainMenuButton;

    HWND m_map1Button;
    HWND m_map2Button;
    HWND m_map3Button;

    ImageContainer* m_map1Image;
    ImageContainer* m_map2Image;
    ImageContainer* m_map3Image;

    int m_mapIndex = 0;
    SnakeGame::Difficulty m_difficulty;
};
