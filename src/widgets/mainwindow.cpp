#include "mainwindow.h"

#include "services/settings.h"

MainWindow::MainWindow()
    : IWindow<MainWindow>()
    , m_snakeGame(std::make_unique<SnakeGame>())
    , m_mainMenu(std::make_unique<MainMenu>())
    , m_changeMode(std::make_unique<ChangeModeWidget>())
    , m_records(std::make_unique<RecordsWindow>())
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
                            0, 0, m_windowWidth, m_windowHeight,
                            m_hwnd, (HMENU)SnakeGameId, (HBRUSH)(COLOR_BTNFACE + 1));
        m_mainMenu->create(L"Main Menu", WS_CHILD | WS_VISIBLE, 0,
                           0, 0, m_windowWidth, m_windowHeight,
                           m_hwnd, (HMENU)MainMenuId, (HBRUSH)(COLOR_WINDOW + 1));
        m_changeMode->create(L"Change Mode", WS_CHILD, 0, // is hidden initially
                             0, 0, m_windowWidth, m_windowHeight,
                             m_hwnd, (HMENU)ChangeModeId, (HBRUSH)(COLOR_WINDOW + 1));
        m_records->create(L"Records Window", WS_CHILD, 0, // is hidden initially
                            0, 0, m_windowWidth, m_windowHeight,
                            m_hwnd, (HMENU)RecordsId, (HBRUSH)(COLOR_WINDOW + 1));

        m_font = CreateFont(48, 0, 0, 0, FW_BOLD,
                            FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
                            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                            DEFAULT_PITCH | FF_DONTCARE, L"Arial Rounded MT");

        m_smallFont = CreateFont(24, 0, 0, 0, FW_BOLD,
                                 FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
                                 OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                 DEFAULT_PITCH | FF_DONTCARE, L"Arial Rounded MT");

        EnumChildWindows(m_hwnd, (WNDENUMPROC)&MainWindow::setFont,
                         (LPARAM)m_font);
        SendMessage(m_mainMenu->nameEdit(), WM_SETFONT, (WPARAM)m_smallFont, TRUE);
        SendMessage(m_records->difficultyComboBox(), WM_SETFONT, (WPARAM)m_smallFont, TRUE);
        SendMessage(m_records->mapComboBox(), WM_SETFONT, (WPARAM)m_smallFont, TRUE);
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
    case StartGame: {
        showWindow(m_snakeGame->window());
        m_snakeGame->startGame();
        return 0;
    }
    case EndGame: {
        auto score = static_cast<int>(wParam);
        m_records->addRecord(Settings::fromUtf8(appSettings->playerName()), score,
                             appSettings->difficulty(), appSettings->mapIndex());
        return 0;
    }
    case ToMainMenu:{
        showWindow(m_mainMenu->window());
        return 0;
    }
    case ToChangeMode: {
        showWindow(m_changeMode->window());
        return 0;
    }
    case ToRecords: {
        showWindow(m_records->window());
        return 0;
    }
    case SetObstacles: {
        auto obstacles = reinterpret_cast<std::vector<Point>*>(wParam);
        m_snakeGame->setObstacles(*obstacles);
        return 0;
    }
    case WM_ERASEBKGND:
        return TRUE;
    case WM_DESTROY:
        DeleteObject(m_smallFont);
        DeleteObject(m_font);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow::showWindow(HWND window)
{
    ShowWindow(m_snakeGame->window(), SW_HIDE);
    ShowWindow(m_mainMenu->window(), SW_HIDE);
    ShowWindow(m_changeMode->window(), SW_HIDE);
    ShowWindow(m_records->window(), SW_HIDE);

    ShowWindow(window, SW_SHOW);
}
