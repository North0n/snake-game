#include "snakegame.h"

#include "mainwindow.h"
#include "services/settings.h"

#include <sstream>

PCWSTR SnakeGame::className() const
{
    return L"Snake Game Window Class";
}

LRESULT SnakeGame::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        SetFocus(m_hwnd);
        return 0;
    }
    case WM_TIMER: {
        switch (wParam) {
        case SnakeGameTimerId: {
            // Don't do anything if game is paused
            if (m_snakeDirection == Vector{0, 0}) {
                return 0;
            }

            // Get next position of snake head
            auto nextPoint = m_gridAligner.toCellCoords(m_snake->position() + m_snakeDirection * CellSize);

            // Check if snake is going to bump into something
            if (!m_isVulnerable && (m_snake->contains(nextPoint) ||
                bumpIntoObstacle(nextPoint))) {
                if (--m_lives < 0) {
                    KillTimer(m_hwnd, SnakeGameTimerId);

                    std::wstringstream ss;
                    ss << L"���� ��������! ��� ����: " << m_score << L". ������?";
                    auto res = MessageBox(m_hwnd, ss.str().c_str(), L"���� ��������", MB_OK | MB_YESNO);
                    SendMessage(GetParent(m_hwnd), MainWindow::EndGame, m_score, 0);
                    if (res == IDYES) {
                        startGame();
                    } else {
                        SendMessage(GetParent(m_hwnd), MainWindow::ToMainMenu, 0, 0);
                    }
                    return 0;
                } else {
                    m_isVulnerable = true;
                    m_vulnerabilityTimeLeft = InvulnerabilityTime;
                    SetTimer(m_hwnd, InvulnerabilityTimerId, InvulnerabilityTime / LifeTimeDivisor, nullptr);
                }
            }

            // Snake isn't going to bump into anything, so move it
            m_snake->moveOn(m_snakeDirection * CellSize, m_windowWidth, m_windowHeight);

            // Check if snake ate food or heart
            if (m_apple->position() == nextPoint) {
                m_snake->appendSegment();
                ++m_score;
                m_apple = std::make_unique<Apple>(generateEatablePosition(), CellSize);
                if (m_snakeGameTimerInterval > DifficultyParams[static_cast<int>(appSettings->difficulty())].minInterval) {
                    m_snakeGameTimerInterval -= DifficultyParams[static_cast<int>(appSettings->difficulty())].intervalStep;
                    if (m_isAccelerated) {
                        SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval / 2, nullptr);
                    } else {
                        SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval, nullptr);
                    }
                }

                if (m_score % HeartFrequency == 0) {
                    m_heart = std::make_unique<Heart>(generateEatablePosition(), CellSize);
                    m_heartTimeLeft = HeartLifeTime;
                    SetTimer(m_hwnd, HeartTimerId, HeartLifeTime / LifeTimeDivisor, nullptr);
                }
            } else if (m_heart && m_heart->position() == nextPoint) {
                KillTimer(m_hwnd, HeartTimerId);
                m_heart.reset();
                m_lives = min(m_lives + 1, MaxLivesCount);
            }
            m_movedInDirection = true;
            break;
        }
        case HeartTimerId: {
            m_heartTimeLeft -= HeartLifeTime / LifeTimeDivisor;
            if (m_heartTimeLeft <= 0) {
                KillTimer(m_hwnd, HeartTimerId);
                m_heart.reset();
            }
            break;
        }
        case InvulnerabilityTimerId: {
            m_vulnerabilityTimeLeft -= InvulnerabilityTime / LifeTimeDivisor;
            if (m_vulnerabilityTimeLeft <= 0) {
                KillTimer(m_hwnd, InvulnerabilityTimerId);
                m_isVulnerable = false;
            }
            break;
        }
        }
        InvalidateRect(m_hwnd, nullptr, TRUE);
        return 0;
    }
    case WM_KEYDOWN: {
        Vector direction{};
        switch (wParam) {
        case VK_UP:
            direction = {0, -1};
            break;
        case VK_DOWN:
            direction = {0, 1};
            break;
        case VK_LEFT:
            direction = {-1, 0};
            break;
        case VK_RIGHT:
            direction = {1, 0};
            break;
        case VK_SPACE:
            if (m_snakeDirection == Vector{}) {
                return 0;
            }
            if (m_isGamePaused) {
                if (m_isAccelerated) {
                    SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval / 2, nullptr);
                } else {
                    SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval, nullptr);
                }
                if (m_heartTimeLeft > 0) {
                    SetTimer(m_hwnd, HeartTimerId, HeartLifeTime / LifeTimeDivisor, nullptr);
                }
                if (m_vulnerabilityTimeLeft > 0) {
                    SetTimer(m_hwnd, InvulnerabilityTimerId, InvulnerabilityTimerId / LifeTimeDivisor, nullptr);
                }
            } else {
                KillTimer(m_hwnd, SnakeGameTimerId);
                KillTimer(m_hwnd, HeartTimerId);
                KillTimer(m_hwnd, InvulnerabilityTimerId);
            }
            m_isGamePaused = !m_isGamePaused;
            InvalidateRect(m_hwnd, nullptr, TRUE);
            return 0;
        case VK_SHIFT: {
            if (m_isAccelerated) {
                SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval, nullptr);
            } else {
                SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval / 2, nullptr);
            }
            m_isAccelerated = !m_isAccelerated;
            return 0;
        }
        case VK_ESCAPE: {
            KillTimer(m_hwnd, SnakeGameTimerId);
            auto res = MessageBox(m_hwnd, L"�� �������, ��� ������ ����� � ������� ����?", L"����� � ������� ����", MB_OK | MB_YESNO);
            if (res == IDYES) {
                SendMessage(GetParent(m_hwnd), MainWindow::ToMainMenu, 0, 0);
            } else {
                SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval, nullptr);
                SetFocus(m_hwnd);
            }
            return 0;
        }
        default:
            return 0;
        }
        if (direction != -m_snakeDirection && m_movedInDirection && !m_isGamePaused) {
            m_snakeDirection = direction;
            m_movedInDirection = false;
        }
        return 0;
    }
    case WM_CREATE: {
        EnumChildWindows(m_hwnd, (WNDENUMPROC)&SnakeGame::setFont,
                         (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

        RECT rect;
        GetWindowRect(m_hwnd, &rect);
        m_windowWidth  = rect.right - rect.left;
        m_windowHeight = rect.bottom - rect.top;
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(m_hwnd, &ps);
        Gdiplus::Graphics graphics(hdc);
        Gdiplus::Bitmap bitmap(ps.rcPaint.right - ps.rcPaint.left,
                               ps.rcPaint.bottom - ps.rcPaint.top, &graphics);
        Gdiplus::Graphics graphicsBack(&bitmap);
        graphicsBack.Clear(Gdiplus::Color::White);
        m_painter = std::make_unique<GdiPlusPainter>(graphicsBack);
        m_painter->draw(*m_snake, m_snakeDirection, m_isVulnerable);
        m_painter->draw(*m_apple);
        if (m_heart) {
            m_painter->draw(*m_heart);
            m_painter->drawHeartTimeLeft(m_heartTimeLeft, HeartLifeTime);
        }
        if (m_isVulnerable) {
            m_painter->drawInvulnerabilityTimeLeft(m_vulnerabilityTimeLeft, InvulnerabilityTime);
        }
        m_painter->drawScore(m_score);
        m_painter->drawLives(m_lives);
        m_painter->draw(m_obstacles);
        if (m_isGamePaused) {
            m_painter->drawPause(m_windowWidth, m_windowHeight);
        }
        m_painter = nullptr;
        graphics.DrawImage(&bitmap, 0, 0, 0, 0, ps.rcPaint.right - ps.rcPaint.left,
                           ps.rcPaint.bottom - ps.rcPaint.top, Gdiplus::UnitPixel);
        EndPaint(m_hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return TRUE;
    case WM_DESTROY: {
        KillTimer(m_hwnd, SnakeGameTimerId);
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

Point SnakeGame::generateEatablePosition() const
{
    Point point{};
    do {
        point.x = m_gridAligner.toCellCoords(m_distribution(m_random) % (m_windowWidth - 2 * CellSize) + CellSize);
        point.y = m_gridAligner.toCellCoords(m_distribution(m_random) % (m_windowHeight - 2 * CellSize) + CellSize);
    } while (m_snake->contains(point) || bumpIntoObstacle(point));

    return point;
}

void SnakeGame::startGame()
{
    m_snake = std::make_unique<Snake>(Point(m_gridAligner.toCellCoords(m_windowWidth / 2),
                                            m_gridAligner.toCellCoords(m_windowHeight / 2)),
                                      HeadSideLength, CellSize);
    m_apple = std::make_unique<Apple>(generateEatablePosition(), CellSize);
    m_heart = nullptr;

    m_snakeDirection         = {0, 0};
    m_movedInDirection       = true;
    m_score                  = 0;
    m_isGamePaused           = false;
    m_isAccelerated          = false;
    m_isVulnerable           = false;
    m_heartTimeLeft          = 0;
    m_vulnerabilityTimeLeft  = 0;
    m_lives                  = 0;
    m_snakeGameTimerInterval = DifficultyParams[static_cast<int>(appSettings->difficulty())].initialInterval;

    SetFocus(m_hwnd);
    SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval, nullptr);
    InvalidateRect(m_hwnd, nullptr, TRUE);
}

bool SnakeGame::bumpIntoObstacle(const Point& point) const
{
    return std::any_of(m_obstacles.begin(), m_obstacles.end(),
                       [&point](const auto& obstacle) { return obstacle == point; });
}
