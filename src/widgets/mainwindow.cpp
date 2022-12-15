#include "mainwindow.h"

MainWindow::MainWindow()
    : IWindow<MainWindow>()
    , m_snakeGame(std::make_unique<SnakeGame>())
{
}

PCWSTR MainWindow::className() const
{
    return L"Snake Game Window Class";
}

LRESULT MainWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE: {
        RECT rect;
        GetWindowRect(m_hwnd, &rect);
        m_snakeGame->create(L"Snake Game", WS_CHILD | WS_VISIBLE, 0,
                            10, 10, 1200, 600,
                            m_hwnd, (HBRUSH)(COLOR_BTNFACE + 1));
        SetFocus(m_snakeGame->window());
        EnumChildWindows(m_hwnd, (WNDENUMPROC)&MainWindow::setFont,
                         (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(m_hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BTNSHADOW + 1));
        EndPaint(m_hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
