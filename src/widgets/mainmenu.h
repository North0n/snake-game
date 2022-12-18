#pragma once

#include "iwindow.h"

class MainMenu : public IWindow<MainMenu>
{
public:
    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    [[nodiscard]] HWND nameEdit() const { return m_nameEdit; }

private:
    static constexpr inline int PlayButtonId       = 1;
    static constexpr inline int ExitButtonId       = 2;
    static constexpr inline int ChangeModeButtonId = 3;
    static constexpr inline int RecordsButtonId    = 4;
    static constexpr inline int NameEditId         = 5;

    static void setWindowRegion(HWND hwnd);

    HWND m_playButton;
    HWND m_exitButton;
    HWND m_changeModeButton;
    HWND m_recordsButton;
    HWND m_nameEdit;
};
