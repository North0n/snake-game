#pragma once

#include <Windows.h>

namespace Gdiplus {
class Rect;
class GraphicsPath;
}

class RegisterEditControl
{
public:
    RegisterEditControl();
    ~RegisterEditControl();

private:
    void registerClass();
    void unregisterClass();

    static LRESULT CALLBACK editRounderBorderWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static void drawRoundedBorder(HWND hWnd, int radius = 10);

    static void getChildRect(HWND hChild, LPRECT rc);

    static void getRoundRectPath(Gdiplus::GraphicsPath* pPath, const Gdiplus::Rect& r, int dia);
};
