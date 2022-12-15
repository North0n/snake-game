#pragma once

#include "entities/apple.h"
#include "entities/snake.h"
#include "iwindow.h"
#include "services/gdipluspainter.h"
#include "services/gridaligner.h"

#include <chrono>
#include <memory>
#include <random>

class SnakeGame : public IWindow<SnakeGame>
{
public:
    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    static constexpr inline int HeadSideLength = 40;
    static constexpr inline int CellSize       = 40;

    static constexpr inline int SnakeGameTimerId       = 1;
    static constexpr inline int SnakeGameTimerInterval = 64; // in milliseconds

    static inline std::mt19937 m_random{std::random_device{}()};
    static inline std::uniform_int_distribution<> m_distribution{};

    [[nodiscard]] Point generateApplePosition() const;

    GridAligner m_gridAligner{CellSize};

    int m_windowWidth;
    int m_windowHeight;

    Vector m_snakeDirection{1, 0};

    std::unique_ptr<Snake> m_snake;
    std::unique_ptr<Apple> m_apple;
    std::unique_ptr<GdiPlusPainter> m_painter;
};
