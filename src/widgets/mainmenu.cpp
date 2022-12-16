#include "mainmenu.h"

PCWSTR MainMenu::className() const
{
    return L"Main Menu Window Class";
}

LRESULT MainMenu::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE: {
        m_playButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Играть",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      500, 100, 200, 100, m_hwnd,
                                      (HMENU)PlayButtonId, nullptr, nullptr);
        setWindowRegion(m_playButton);

        m_exitButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Выход",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      500, 250, 200, 100, m_hwnd,
                                      (HMENU)ExitButtonId, nullptr, nullptr);
        setWindowRegion(m_exitButton);
        return 0;
    }
    case WM_DRAWITEM: {
        auto item = (LPDRAWITEMSTRUCT)lParam;
        switch (wParam) {
        case PlayButtonId:
        case ExitButtonId: {
            RECT rcClient;
            GetClientRect(item->hwndItem, &rcClient);
            FillRect(item->hDC, &item->rcItem, GetSysColorBrush(COLOR_WINDOW));

            auto initialRegion = CreateRectRgn(0, 0, 0, 0);
            GetWindowRgn(item->hwndItem, initialRegion);
            auto brush = CreateSolidBrush(RGB(41, 245, 92));
            FillRgn(item->hDC, initialRegion, brush);
            DeleteObject(brush);
            DeleteObject(initialRegion);

            SetBkMode(item->hDC, TRANSPARENT);
            if (item->itemState & ODS_SELECTED) {
                SetTextColor(item->hDC, RGB(255, 255, 255));
            }

            int len     = GetWindowTextLength(item->hwndItem);
            auto lpBuff = new wchar_t[len + 1];
            GetWindowText(item->hwndItem, lpBuff, len + 1);
            DrawText(item->hDC, lpBuff, len, &item->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            delete[] lpBuff;
        }
        }
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainMenu::setWindowRegion(HWND hwnd)
{
    RECT rcClient;
    GetClientRect(hwnd, &rcClient);
    rcClient.left += 2;
    rcClient.top += 2;
    auto hrgnButton = CreateRoundRectRgn(rcClient.left, rcClient.top,
                                         rcClient.right, rcClient.bottom,
                                         20, 20);
    SetWindowRgn(hwnd, hrgnButton, TRUE);
}
