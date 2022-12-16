#include "mainwindow.h"

MainWindow::MainWindow()
    : IWindow<MainWindow>()
    , m_snakeGame(std::make_unique<SnakeGame>())
    , m_mainMenu(std::make_unique<MainMenu>())
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
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        m_windowWidth  = rc.right - rc.left;
        m_windowHeight = rc.bottom - rc.top;
        m_snakeGame->create(L"Snake Game", WS_CHILD, 0, // is hidden initially
                            0, 0, 1200, 600,
                            m_hwnd, (HMENU)SnakeGameId, (HBRUSH)(COLOR_BTNFACE + 1));
        m_mainMenu->create(L"Main Menu", WS_CHILD | WS_VISIBLE, 0,
                           0, 0, m_windowWidth, m_windowHeight,
                           m_hwnd, (HMENU)MainMenuId, (HBRUSH)(COLOR_WINDOW + 1));

        HFONT hFont = CreateFont(48, 0, 0, 0, FW_BOLD,
                                 FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
                                 OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                 DEFAULT_PITCH | FF_DONTCARE, L"Arial Rounded MT");
        EnumChildWindows(m_hwnd, (WNDENUMPROC)&MainWindow::setFont,
                         (LPARAM)hFont);
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(m_hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BTNSHADOW + 1));
        Gdiplus::Graphics graphics(hdc);
        EndPaint(m_hwnd, &ps);
        return 0;
    }
    case StartGameMessage: {
        ShowWindow(m_mainMenu->window(), SW_HIDE);
        ShowWindow(m_snakeGame->window(), SW_SHOW);
        SetFocus(m_snakeGame->window());
        SendMessage(m_snakeGame->window(), SnakeGame::StartGameMessage, 0, 0);
    }
    case WM_ERASEBKGND:
        return TRUE;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
