#include "snakegame.h"

PCWSTR SnakeGame::className() const
{
    return L"Snake Game";
}

LRESULT SnakeGame::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        SetFocus(m_hwnd);
        return 0;
    }
    case WM_TIMER: {
        auto nextPoint = m_gridAligner.toCellCoords(m_snake->position() + m_snakeDirection * CellSize);
        if (m_apple->position().isBetween(m_snake->position(), nextPoint)) {
            m_snake->appendSegment();
            m_apple = std::make_unique<Apple>(generateApplePosition(), CellSize);
        }
        m_snake->moveOn(m_snakeDirection * CellSize);
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
        }
        if (direction != Vector{} && direction != -m_snakeDirection) {
            m_snakeDirection = direction;
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

        m_snake = std::make_unique<Snake>(Point(m_gridAligner.toCellCoords(m_windowWidth / 2),
                                                m_gridAligner.toCellCoords(m_windowHeight / 2)),
                                          HeadSideLength, CellSize);
        m_apple = std::make_unique<Apple>(generateApplePosition(), CellSize);

        SetTimer(m_hwnd, SnakeGameTimerId, SnakeGameTimerInterval, nullptr);
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
        point.x = m_distribution(m_random) % (m_windowWidth - 2 * CellSize) + CellSize;
        point.y = m_distribution(m_random) % (m_windowHeight - 2 * CellSize) + CellSize;
    } while (m_snake->contains(point));

    return m_gridAligner.toCellCoords(point);
}
