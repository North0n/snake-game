#pragma once

#include "iwindow.h"

class MainMenu : public IWindow<MainMenu>
{
public:
    [[nodiscard]] PCWSTR className() const override;
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    static constexpr inline int PlayButtonId = 1;
    static constexpr inline int ExitButtonId = 2;

    static void setWindowRegion(HWND hwnd);

    HWND m_playButton;
    HWND m_exitButton;
};
