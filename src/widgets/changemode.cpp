#include "changemode.h"

#include "mainwindow.h"

PCWSTR ChangeModeWidget::className() const
{
    return L"Change Mode Window Class";
}

LRESULT ChangeModeWidget::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
        m_easyButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Легко",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      200, 50, 200, 75, m_hwnd,
                                      (HMENU)EasyButtonId, nullptr, nullptr);
        setWindowRegion(m_easyButton);

        m_mediumButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Средне",
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        500, 50, 200, 75, m_hwnd,
                                        (HMENU)MediumButtonId, nullptr, nullptr);
        setWindowRegion(m_mediumButton);

        m_hardButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Сложно",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      800, 50, 200, 75, m_hwnd,
                                      (HMENU)HardButtonId, nullptr, nullptr);
        setWindowRegion(m_hardButton);

        m_mainMenuButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Назад",
                                          WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                          400, 500, 400, 75, m_hwnd,
                                          (HMENU)MainMenuButtonId, nullptr, nullptr);
        setWindowRegion(m_mainMenuButton);
        return 0;
    case WM_DRAWITEM: {
        auto item = (LPDRAWITEMSTRUCT)lParam;
        RECT rcClient;
        GetClientRect(item->hwndItem, &rcClient);
        FillRect(item->hDC, &item->rcItem, GetSysColorBrush(COLOR_WINDOW));

        auto initialRegion = CreateRectRgn(0, 0, 0, 0);
        GetWindowRgn(item->hwndItem, initialRegion);
        auto color = RGB(41, 245, 92);
        if (item->CtlID - 1 == static_cast<UINT>(m_difficulty)) {
            color = RGB(255, 0, 0);
        }
        auto brush = CreateSolidBrush(color);
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

        return 0;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case EasyButtonId:
        case MediumButtonId:
        case HardButtonId: {
            auto difficulty = LOWORD(wParam) - 1;
            setDifficulty(static_cast<SnakeGame::Difficulty>(difficulty));
            SendMessage(GetParent(m_hwnd), MainWindow::Message::ChangeDifficulty, difficulty, 0);
            InvalidateRect(m_hwnd, nullptr, true);
            return 0;
        }
        case MainMenuButtonId: {
            SendMessage(GetParent(m_hwnd), MainWindow::Message::ToMainMenu, 0, 0);
            return 0;
        }
        }
    }
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void ChangeModeWidget::setWindowRegion(HWND hwnd)
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
