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
    case WM_CREATE: {
        EnumChildWindows(m_hwnd, (WNDENUMPROC)&SnakeGame::setFont,
                         (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

        RECT rect;
        GetWindowRect(m_hwnd, &rect);
        m_windowWidth  = rect.right - rect.left;
        m_windowHeight = rect.bottom - rect.top;

        m_snake = std::make_unique<Snake>(Point(m_windowWidth / 2, m_windowHeight / 2), HeadSideLength, BodySideLength);
        m_apple = std::make_unique<Apple>(generateApplePosition(), AppleSideLength);
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(m_hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        m_painter = std::make_unique<GdiPlusPainter>(hdc);
        m_painter->draw(*m_snake);
        m_painter->draw(*m_apple);
        m_painter = nullptr;
        EndPaint(m_hwnd, &ps);
        return 0;
    }
    case WM_DESTROY: {
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
        point.x = m_distribution(m_random) % (m_windowWidth - 2 * AppleSideLength) + AppleSideLength;
        point.y = m_distribution(m_random) % (m_windowHeight - 2 * AppleSideLength) + AppleSideLength;
    } while (m_snake->contains(point));

    return point;
}
