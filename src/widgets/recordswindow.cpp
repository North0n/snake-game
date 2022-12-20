#include "recordswindow.h"

#include "json.hpp"
#include "mainwindow.h"
#include "services/settings.h"

#include <CommCtrl.h>
#include <filesystem>
#include <fstream>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

PCWSTR RecordsWindow::className() const
{
    return L"Records Window Class";
}

LRESULT RecordsWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE: {
        m_mainMenuButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Назад",
                                          WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                          240, 500, 340, 75, m_hwnd,
                                          (HMENU)MainMenuButtonId, nullptr, nullptr);
        setWindowRegion(m_mainMenuButton);

        m_clearButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Очистить",
                                       WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                       610, 500, 350, 75, m_hwnd,
                                       (HMENU)ClearButtonId, nullptr, nullptr);
        setWindowRegion(m_clearButton);
        setWindowRegion(m_clearButton);
        setWindowRegion(m_clearButton);

        m_difficultyComboBox = CreateWindow(WC_COMBOBOX, L"Сложность",
                                            CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                            650, 25, 150, 20, m_hwnd, (HMENU)DifficultyComboBoxId, nullptr,
                                            nullptr);
        SendMessage(m_difficultyComboBox, CB_ADDSTRING, 0, (LPARAM)L"Все");
        SendMessage(m_difficultyComboBox, CB_ADDSTRING, 0, (LPARAM)L"Легкая");
        SendMessage(m_difficultyComboBox, CB_ADDSTRING, 0, (LPARAM)L"Средняя");
        SendMessage(m_difficultyComboBox, CB_ADDSTRING, 0, (LPARAM)L"Сложная");
        SendMessage(m_difficultyComboBox, CB_SETCURSEL, 0, 0);

        m_nameComboBox = CreateWindow(WC_COMBOBOX, L"Имя",
                                      CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                      350, 25, 200, 20, m_hwnd, (HMENU)NameComboBoxId, nullptr,
                                      nullptr);

        m_mapComboBox = CreateWindow(WC_COMBOBOX, L"Карта",
                                     CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                     800, 25, 150, 20, m_hwnd, (HMENU)MapComboBoxId, nullptr,
                                     nullptr);
        SendMessage(m_mapComboBox, CB_ADDSTRING, 0, (LPARAM)L"Все");
        SendMessage(m_mapComboBox, CB_ADDSTRING, 0, (LPARAM)L"Карта 1");
        SendMessage(m_mapComboBox, CB_ADDSTRING, 0, (LPARAM)L"Карта 2");
        SendMessage(m_mapComboBox, CB_ADDSTRING, 0, (LPARAM)L"Карта 3");
        SendMessage(m_mapComboBox, CB_SETCURSEL, 0, 0);

        setFileName(m_fileName);
        return 0;
    }
    case WM_DRAWITEM: {
        auto item = (LPDRAWITEMSTRUCT)lParam;
        RECT rcClient;
        GetClientRect(item->hwndItem, &rcClient);
        FillRect(item->hDC, &item->rcItem, GetSysColorBrush(COLOR_WINDOW));

        auto initialRegion = CreateRectRgn(0, 0, 0, 0);
        GetWindowRgn(item->hwndItem, initialRegion);
        auto color = RGB(41, 245, 92);
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
        DrawText(item->hDC, lpBuff, len, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        delete[] lpBuff;

        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(m_hwnd, &ps);
        Gdiplus::Graphics graphics(hdc);
        Gdiplus::Bitmap bitmap(ps.rcPaint.right,ps.rcPaint.bottom, &graphics);
        Gdiplus::Graphics graphicsBack(&bitmap);
        graphicsBack.Clear(Gdiplus::Color::White);

        m_painter = std::make_unique<GdiPlusPainter>(graphicsBack);
        m_painter->draw(m_records, m_filterDifficulty, m_filterMapIndex, m_filterName, RecordsOnPage);
        m_painter = nullptr;
        graphics.DrawImage(&bitmap, 0, 0, 0, 0,
                           ps.rcPaint.right, ps.rcPaint.bottom, Gdiplus::UnitPixel);

        EndPaint(m_hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return true;
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case MainMenuButtonId: {
            SendMessage(GetParent(m_hwnd), MainWindow::Message::ToMainMenu, 0, 0);
            return 0;
        }
        case DifficultyComboBoxId: {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                auto index = SendMessage(m_difficultyComboBox, CB_GETCURSEL, 0, 0);
                if (index == 0) {
                    m_filterDifficulty = std::nullopt;
                } else {
                    m_filterDifficulty = static_cast<Difficulty>(index - 1);
                }
                RECT rc{250, 57, 950, 475};
                InvalidateRect(m_hwnd, &rc, true);
            }
            return 0;
        }
        case MapComboBoxId: {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                auto index = SendMessage(m_mapComboBox, CB_GETCURSEL, 0, 0);
                if (index == 0) {
                    m_filterMapIndex = std::nullopt;
                } else {
                    m_filterMapIndex = index - 1;
                }
                RECT rc{250, 57, 950, 475};
                InvalidateRect(m_hwnd, &rc, true);
            }
            return 0;
        }
        case NameComboBoxId: {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                auto index = SendMessage(m_nameComboBox, CB_GETCURSEL, 0, 0);
                if (index == 0) {
                    m_filterName = std::nullopt;
                } else {
                    int len     = SendMessage(m_nameComboBox, CB_GETLBTEXTLEN, index, 0);
                    auto lpBuff = new wchar_t[len + 1];
                    SendMessage(m_nameComboBox, CB_GETLBTEXT, index, (LPARAM)lpBuff);
                    m_filterName = lpBuff;
                    delete[] lpBuff;
                }
                RECT rc{250, 57, 950, 475};
                InvalidateRect(m_hwnd, &rc, true);
            }
            return 0;
        }
        case ClearButtonId: {
            auto res = MessageBox(m_hwnd, L"Вы действительно хотите очистить таблицу рекордов?",
                                  L"Подтверждение", MB_YESNO);
            if (res != IDYES) {
                return 0;
            }
            clearRecords();
            m_filterDifficulty = std::nullopt;
            m_filterMapIndex = std::nullopt;
            SendMessage(m_difficultyComboBox, CB_SETCURSEL, 0, 0);
            SendMessage(m_mapComboBox, CB_SETCURSEL, 0, 0);
            InvalidateRect(m_hwnd, nullptr, true);
            return 0;
        }
        }
        return 0;
    }
    }
    
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void RecordsWindow::addRecord(const Record& record)
{
    auto elementGreater = m_records.end();
    auto sameElement = m_records.end();
    for (auto it = m_records.begin(); it != m_records.end(); ++it) {
        if (it->score > record.score && elementGreater == m_records.end()) {
            elementGreater = it;
        }
        if (it->difficulty == record.difficulty && it->mapIndex == record.mapIndex && it->name == record.name) {
            sameElement = it;
        }
    }

    if (sameElement != m_records.end()) {
        if (sameElement->score < record.score) {
            m_records.insert(elementGreater, record);
            m_records.erase(sameElement);
        } else {
            return;
        }
    } else {
        m_records.insert(elementGreater, record);
        if (!m_names.contains(record.name)) {
            m_names.insert(record.name);
            SendMessage(m_nameComboBox, CB_ADDSTRING, 0, (LPARAM)record.name.c_str());
        }
    }

    nlohmann::json json;
    std::transform(m_records.begin(), m_records.end(), std::back_inserter(json), [](const Record& r) {
        return nlohmann::json{
            {"name", Settings::toUtf8(r.name)},
            {"score", r.score},
            {"difficulty", r.difficulty},
            {"mapIndex", r.mapIndex}};
    });

    std::ofstream file(m_fileName);
    file << std::setw(4) << json;
}

void RecordsWindow::clearRecords()
{
    m_records.clear();
    std::ofstream file(m_fileName);
    file << "[]";
}

void RecordsWindow::setFileName(const std::string& fileName)
{
    m_fileName = fileName;
    m_records.clear();

    if (!std::filesystem::exists(m_fileName)) {
        return;
    }

    std::ifstream file(m_fileName);
    nlohmann::json json;
    file >> json;

    m_names.clear();
    for (const auto& record : json) {
        Record r;
        r.name       = Settings::fromUtf8(record["name"].get<std::string>());
        r.score      = record["score"].get<int>();
        r.difficulty = static_cast<Difficulty>(record["difficulty"].get<int>());
        r.mapIndex   = record["mapIndex"].get<int>();
        m_records.push_back(r);
        m_names.insert(r.name);
    }

    SendMessage(m_nameComboBox, CB_RESETCONTENT, 0, 0);
    SendMessage(m_nameComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Все"));
    for (const auto& name : m_names) {
        SendMessage(m_nameComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(name.c_str()));
    }
    SendMessage(m_nameComboBox, CB_SETCURSEL, 0, 0);
}

void RecordsWindow::setWindowRegion(HWND hwnd)
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
