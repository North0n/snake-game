#pragma once

#include <Windows.h>

template<typename DerivedWindow>
class IWindow
{
public:
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DerivedWindow *pThis = nullptr;
        if (uMsg == WM_NCCREATE) {
            auto pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
            pThis = reinterpret_cast<DerivedWindow *>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        } else {
            pThis = reinterpret_cast<DerivedWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis) {
            return pThis->handleMessage(uMsg, wParam, lParam);
        } else {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BOOL create(
            PCWSTR lpWindowName,
            DWORD dwStyle,
            DWORD dwExStyle = 0,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int width = CW_USEDEFAULT,
            int height = CW_USEDEFAULT,
            HWND hWndParent = nullptr,
            HBRUSH hbrBackground = nullptr)
    {
        WNDCLASS wc{
                .lpfnWndProc   = DerivedWindow::windowProc,
                .hInstance     = GetModuleHandle(nullptr),
                .hbrBackground = hbrBackground,
                .lpszClassName = className()};

        RegisterClass(&wc);
        m_hwnd = CreateWindowEx(dwExStyle, className(), lpWindowName, dwStyle, x, y, width, height,
                                hWndParent, nullptr, wc.hInstance, this);

        return (m_hwnd ? TRUE : FALSE);
    }

    BOOL create(const WNDCLASS &wc,
                PCWSTR lpWindowName,
                DWORD dwStyle,
                DWORD dwExStyle,
                int x = CW_USEDEFAULT,
                int y = CW_USEDEFAULT,
                int width = CW_USEDEFAULT,
                int height = CW_USEDEFAULT,
                HWND hWndParent = nullptr,
                HMENU hMenu = nullptr)
    {
        RegisterClass(&wc);
        m_hwnd = CreateWindowEx(dwExStyle, className(), lpWindowName, dwStyle, x, y, width, height,
                                hWndParent, hMenu, wc.hInstance, this);

        return (m_hwnd ? TRUE : FALSE);
    }

    [[nodiscard]] HWND window() const { return m_hwnd; }

    [[nodiscard]] virtual PCWSTR className() const = 0;

    virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

protected:
    static bool CALLBACK setFont(HWND child, LPARAM font);

    HWND m_hwnd{nullptr};
};

template<typename DerivedWindow>
bool CALLBACK IWindow<DerivedWindow>::setFont(HWND child, LPARAM font)
{
    SendMessage(child, WM_SETFONT, font, true);
    return true;
}
