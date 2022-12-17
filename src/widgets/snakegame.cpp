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
        if (m_snakeDirection == Vector{0, 0}) {
            return 0;
        }
        auto nextPoint = m_gridAligner.toCellCoords(m_snake->position() + m_snakeDirection * CellSize);
        if (m_snake->contains(nextPoint) ||
            nextPoint.x < 0|| nextPoint.x >= m_windowWidth ||
            nextPoint.y < 0 || nextPoint.y >= m_windowHeight ||
            bumpIntoObstacle(nextPoint)) {
            KillTimer(m_hwnd, SnakeGameTimerId);

            std::wstringstream ss;
            ss << L"Игра окончена! Ваш счёт: " << m_score << L". Заново?";
            auto res = MessageBox(m_hwnd, ss.str().c_str(), L"Игра окончена", MB_OK | MB_YESNO);
            SendMessage(GetParent(m_hwnd), MainWindow::EndGame, m_score, 0);
            if (res == IDYES) {
                startGame();
            } else {
                SendMessage(GetParent(m_hwnd), MainWindow::ToMainMenu, 0, 0);
            }
            return 0;
        }
        if (m_apple->position() == nextPoint) {
            m_snake->appendSegment();
            ++m_score;
            m_apple = std::make_unique<Apple>(generateApplePosition(), CellSize);
            if (m_snakeGameTimerInterval > DifficultyParams[static_cast<int>(appSettings->difficulty())].minInterval) {
                m_snakeGameTimerInterval -= DifficultyParams[static_cast<int>(appSettings->difficulty())].intervalStep;
            }
            KillTimer(m_hwnd, SnakeGameTimerId);
            SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval, nullptr);
        }
        m_snake->moveOn(m_snakeDirection * CellSize);
        m_movedInDirection = true;
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
                SetTimer(m_hwnd, SnakeGameTimerId, m_snakeGameTimerInterval, nullptr);
            } else {
                KillTimer(m_hwnd, SnakeGameTimerId);
            }
            m_isGamePaused = !m_isGamePaused;
            InvalidateRect(m_hwnd, nullptr, TRUE);
            return 0;
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
        m_painter->draw(*m_snake);
        m_painter->draw(*m_apple);
        m_painter->draw(m_score);
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

Point SnakeGame::generateApplePosition() const
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
    m_apple = std::make_unique<Apple>(generateApplePosition(), CellSize);

    m_snakeDirection = {0, 0};
    m_movedInDirection = true;
    m_score = 0;
    m_isGamePaused = false;
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
