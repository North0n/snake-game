#include "recordswindow.h"

PCWSTR RecordsWindow::className() const
{
    return L"Records Window Class";
}

LRESULT RecordsWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE: {
        return 0;
    }
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
