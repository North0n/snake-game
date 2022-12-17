#include "changemode.h"

#include "entities/imagecontainer.h"
#include "mainwindow.h"
#include "services/settings.h"

PCWSTR ChangeModeWidget::className() const
{
    return L"Change Mode Window Class";
}

LRESULT ChangeModeWidget::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
        // Difficulty buttons
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

        // Back button
        m_mainMenuButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Назад",
                                          WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                          400, 500, 400, 75, m_hwnd,
                                          (HMENU)MainMenuButtonId, nullptr, nullptr);
        setWindowRegion(m_mainMenuButton);

        // Map buttons
        m_map1Button = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Карта 1",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      188, 200, 225, 225, m_hwnd,
                                      (HMENU)Map1ButtonId, nullptr, nullptr);
        setWindowRegion(m_map1Button);

        m_map2Button = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Карта 2",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      488, 200, 225, 225, m_hwnd,
                                      (HMENU)Map2ButtonId, nullptr, nullptr);
        setWindowRegion(m_map2Button);

        m_map3Button = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Карта 3",
                                      WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                      788, 200, 225, 225, m_hwnd,
                                      (HMENU)Map3ButtonId, nullptr, nullptr);
        setWindowRegion(m_map3Button);

        m_map1Image = new ImageContainer;
        m_map1Image->load(L"Map1", L"Image");

        m_map2Image = new ImageContainer;
        m_map2Image->load(L"Map2", L"Image");

        m_map3Image = new ImageContainer;
        m_map3Image->load(L"Map3", L"Image");
        return 0;
    case WM_DRAWITEM: {
        auto item = (LPDRAWITEMSTRUCT)lParam;
        RECT rcClient;
        GetClientRect(item->hwndItem, &rcClient);
        FillRect(item->hDC, &item->rcItem, GetSysColorBrush(COLOR_WINDOW));

        auto initialRegion = CreateRectRgn(0, 0, 0, 0);
        GetWindowRgn(item->hwndItem, initialRegion);
        auto color = RGB(41, 245, 92);
        if (item->CtlID - 1 == static_cast<UINT>(appSettings->difficulty()) || item->CtlID - 5 == static_cast<UINT>(appSettings->mapIndex())) {
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
        if (item->CtlID == EasyButtonId || item->CtlID == MediumButtonId || item->CtlID == HardButtonId || item->CtlID == MainMenuButtonId) {
            DrawText(item->hDC, lpBuff, len, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        } else {
            DrawText(item->hDC, lpBuff, len, &rcClient, DT_CENTER | DT_TOP | DT_SINGLELINE);

            Gdiplus::Graphics graphics(item->hDC);
            switch (item->CtlID) {
            case Map1ButtonId:
                graphics.DrawImage(*m_map1Image, 12, 80, 200, 100);
                break;
            case Map2ButtonId:
                graphics.DrawImage(*m_map2Image, 12, 80, 200, 100);
                break;
            case Map3ButtonId:
                graphics.DrawImage(*m_map3Image, 12, 80, 200, 100);
                break;
            }
        }
        delete[] lpBuff;

        return 0;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case EasyButtonId:
        case MediumButtonId:
        case HardButtonId: {
            auto difficulty = LOWORD(wParam) - 1;
            appSettings->set_difficulty(static_cast<Difficulty>(difficulty));
            InvalidateRect(m_hwnd, nullptr, true);
            return 0;
        }
        case MainMenuButtonId: {
            SendMessage(GetParent(m_hwnd), MainWindow::Message::ToMainMenu, 0, 0);
            return 0;
        }
        case Map1ButtonId:
        case Map2ButtonId:
        case Map3ButtonId: {
            appSettings->set_mapIndex(LOWORD(wParam) - 5);
            SendMessage(GetParent(m_hwnd), MainWindow::Message::SetObstacles, reinterpret_cast<WPARAM>(&MapsOfObstacles[appSettings->mapIndex()]), 0);
            InvalidateRect(m_hwnd, nullptr, true);
            return 0;
        }
        }
    }
    case WM_DESTROY: {
        delete m_map1Image;
        delete m_map2Image;
        delete m_map3Image;
        return 0;
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
