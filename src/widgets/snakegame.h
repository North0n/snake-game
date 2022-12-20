#pragma once

#include "entities/apple.h"
#include "entities/heart.h"
#include "entities/snake.h"
#include "iwindow.h"
#include "services/gdipluspainter.h"
#include "services/gridaligner.h"

#include <array>
#include <chrono>
#include <memory>
#include <random>
#include <vector>

class SnakeGame : public IWindow<SnakeGame>
{
public:
    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void setObstacles(const std::vector<Point>& obstacles) { m_obstacles = obstacles; }

    [[nodiscard]] const std::vector<Point>& obstacles() const { return m_obstacles; }

    void startGame();

private:
    static constexpr inline int HeadSideLength = 40;
    static constexpr inline int CellSize       = 40;

    static constexpr inline int SnakeGameTimerId       = 1;
    static constexpr inline int HeartTimerId           = 2;
    static constexpr inline int InvulnerabilityTimerId = 3;

    static constexpr inline int HeartFrequency = 10;
    static constexpr inline int HeartLifeTime  = 3000;
    static constexpr inline int MaxLivesCount  = 3;

    static constexpr inline int InvulnerabilityTime = 1000;

    static constexpr inline int LifeTimeDivisor = 20;

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
        TimerParams{80, 150, 8},
        TimerParams{48, 100, 10}};

    [[nodiscard]] Point generateEatablePosition() const;

    [[nodiscard]] bool bumpIntoObstacle(const Point& point) const;

    GridAligner m_gridAligner{CellSize};

    int m_windowWidth;
    int m_windowHeight;

    bool m_isGamePaused  = false;
    bool m_isAccelerated = false;

    Vector m_snakeDirection{1, 0};
    bool m_movedInDirection{true};
    int m_snakeGameTimerInterval;
    int m_score = 0;
    std::vector<Point> m_obstacles;

    int m_lives                 = 0;
    bool m_isVulnerable         = false;
    int m_heartTimeLeft         = 0;
    int m_vulnerabilityTimeLeft = 0;

    std::unique_ptr<Snake> m_snake;
    std::unique_ptr<Apple> m_apple;
    std::unique_ptr<Heart> m_heart;

    std::unique_ptr<GdiPlusPainter> m_painter;
};
