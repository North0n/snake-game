#pragma once

#include "entities/apple.h"
#include "entities/snake.h"
#include "iwindow.h"
#include "services/gdipluspainter.h"
#include "services/gridaligner.h"

#include <array>
#include <chrono>
#include <memory>
#include <random>

class SnakeGame : public IWindow<SnakeGame>
{
public:
    enum class Difficulty
    {
        Easy = 0,
        Medium,
        Hard
    };

    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void setDifficulty(Difficulty difficulty);

    [[nodiscard]] Difficulty difficulty() const { return m_difficulty; }

    enum Message {
        StartGame = WM_USER + 0
    };

private:
    static constexpr inline int HeadSideLength = 40;
    static constexpr inline int CellSize       = 40;

    static constexpr inline int SnakeGameTimerId = 1;

    static inline std::mt19937 m_random{std::random_device{}()};
    static inline std::uniform_int_distribution<> m_distribution{};

    // in milliseconds
    struct TimerParams
    {
        int minInterval;
        int initialInterval;
        int intervalStep;
    };

    static constexpr std::array<TimerParams, 3> DifficultyParams{
        TimerParams{100, 200, 5},
        TimerParams{80,  150, 8},
        TimerParams{32,  100, 10}
    };

    [[nodiscard]] Point generateApplePosition() const;

    void startGame();

    GridAligner m_gridAligner{CellSize};

    int m_windowWidth;
    int m_windowHeight;

    Vector m_snakeDirection{1, 0};
    bool m_movedInDirection{true};
    int m_snakeGameTimerInterval;
    int m_score                  = 0;
    Difficulty m_difficulty      = Difficulty::Easy;

    std::unique_ptr<Snake> m_snake;
    std::unique_ptr<Apple> m_apple;
    std::unique_ptr<GdiPlusPainter> m_painter;
};
