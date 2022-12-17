#include "mainmenu.h"

#include "mainwindow.h"
#include "services/settings.h"

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
                                      425, 55, 350, 100, m_hwnd,
                                      (HMENU)PlayButtonId, nullptr, nullptr);
        setWindowRegion(m_playButton);

        m_changeModeButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Сменить режим",
                                            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                            425, 195, 350, 100, m_hwnd,
                                            (HMENU)ChangeModeButtonId, nullptr, nullptr);
        setWindowRegion(m_changeModeButton);

        m_recordsButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Рекорды",
                                         WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                         425, 335, 350, 100, m_hwnd,
                                         (HMENU)RecordsButtonId, nullptr, nullptr);
        setWindowRegion(m_recordsButton);

        m_exitButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Выход",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      425, 475, 350, 100, m_hwnd,
                                      (HMENU)ExitButtonId, nullptr, nullptr);
        setWindowRegion(m_exitButton);

        m_nameEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", Settings::fromUtf8(appSettings->playerName()).c_str(),
                                    WS_CHILD | WS_VISIBLE | ES_CENTER | WS_BORDER,
                                    428, 20, 344, 30, m_hwnd,
                                    (HMENU)NameEditId, nullptr, nullptr);
        return 0;
    }
    case WM_DRAWITEM: {
        auto item = (LPDRAWITEMSTRUCT)lParam;
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

        return 0;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case PlayButtonId:
            SendMessage(GetParent(m_hwnd), MainWindow::StartGame, 0, 0);
            break;
        case ExitButtonId:
            SendMessage(GetParent(m_hwnd), WM_CLOSE, 0, 0);
            break;
        case ChangeModeButtonId:
            SendMessage(GetParent(m_hwnd), MainWindow::ToChangeMode, 0, 0);
            break;
        case RecordsButtonId:
            SendMessage(GetParent(m_hwnd), MainWindow::ToRecords, 0, 0);
            break;
        case NameEditId: {
            if (HIWORD(wParam) == EN_CHANGE) {
                int len     = GetWindowTextLength(m_nameEdit);
                auto lpBuff = new wchar_t[len + 1];
                GetWindowText(m_nameEdit, lpBuff, len + 1);
                appSettings->set_playerName(Settings::toUtf8(lpBuff));
                delete[] lpBuff;
            }
            break;
        }
        }
        return 0;
    }
    case WM_DESTROY:
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
