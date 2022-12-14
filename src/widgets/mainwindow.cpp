#include "mainwindow.h"

PCWSTR MainWindow::className() const
{
    return L"Snake Game Window Class";
}

LRESULT MainWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_CREATE:
            EnumChildWindows(m_hwnd, (WNDENUMPROC) &MainWindow::setFont,
                             (LPARAM) GetStockObject(DEFAULT_GUI_FONT));
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
