#pragma once

#include "iwindow.h"
#include "mainmenu.h"
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

    static constexpr inline int StartGameMessage = WM_USER + 0;

private:
    static constexpr inline int SnakeGameId = 1;
    static constexpr inline int MainMenuId  = 2;

    int m_windowWidth;
    int m_windowHeight;

    std::unique_ptr<SnakeGame> m_snakeGame;
    std::unique_ptr<MainMenu> m_mainMenu;
};
