#pragma once

#include "changemode.h"
#include "iwindow.h"
#include "mainmenu.h"
#include "recordswindow.h"
#include "snakegame.h"

#include <memory>

#ifdef _UNICODE
#    pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

class MainWindow : public IWindow<MainWindow>
{
public:
    MainWindow();

    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    enum Message : UINT
    {
        StartGame = WM_USER + 0,
        EndGame,
        ToChangeMode,
        ToMainMenu,
        SetObstacles,
        ToRecords
    };

private:
    static constexpr inline int SnakeGameId  = 1;
    static constexpr inline int MainMenuId   = 2;
    static constexpr inline int ChangeModeId = 3;
    static constexpr inline int RecordsId    = 4;

    int m_windowWidth;
    int m_windowHeight;

    HFONT m_font;
    HFONT m_smallFont;

    std::unique_ptr<SnakeGame> m_snakeGame;
    std::unique_ptr<MainMenu> m_mainMenu;
    std::unique_ptr<ChangeModeWidget> m_changeMode;
    std::unique_ptr<RecordsWindow> m_records;
};
