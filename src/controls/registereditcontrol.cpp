#include "registereditcontrol.h"

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

WNDPROC g_oldEditWndProc = nullptr;

RegisterEditControl::RegisterEditControl()
{
    registerClass();
}

RegisterEditControl::~RegisterEditControl()
{
    unregisterClass();
}

void RegisterEditControl::registerClass()
{
    HWND hEdit       = CreateWindow(L"EDIT", L"", 0, 0, 0, 200, 20, nullptr, nullptr,
                                    GetModuleHandle(nullptr), nullptr);
    g_oldEditWndProc = (WNDPROC)GetClassLongPtr(hEdit, GCLP_WNDPROC);
    SetClassLongPtr(hEdit, GCLP_WNDPROC, (LONG_PTR)editRounderBorderWndProc);
    DestroyWindow(hEdit);
}

void RegisterEditControl::unregisterClass()
{
    HWND hEdit = CreateWindow(L"EDIT", L"", 0, 0, 0, 200, 20, nullptr, nullptr,
                              GetModuleHandle(nullptr), nullptr);
    SetClassLongPtr(hEdit, GCLP_WNDPROC, (LONG_PTR)g_oldEditWndProc);
    DestroyWindow(hEdit);
}

LRESULT RegisterEditControl::editRounderBorderWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_NCPAINT:
        if (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE) {
            drawRoundedBorder(hWnd);
            return 0;
        }
    }
    return CallWindowProc(g_oldEditWndProc, hWnd, uMsg, wParam, lParam);
}

void RegisterEditControl::drawRoundedBorder(HWND hWnd, int radius)
{
    Pen pen(COLORREF(0xFF29f55c));
    SolidBrush brush(Color(255, 255, 255, 255));

    RECT rc = {0};
    getChildRect(hWnd, &rc);
    // the normal EX_CLIENTEDGE is 2 pixels thick.
    // up to a radius of 5, this just works out.
    // for a larger radius, the rectangle must be inflated
    if (radius > 5) {
        int s = radius / 2 - 2;
        InflateRect(&rc, s, s);
    }
    GraphicsPath path;
    getRoundRectPath(&path, Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top),
                     radius * 2);

    HWND hParent = GetParent(hWnd);
    HDC hdc      = GetDC(hParent);
    Graphics graphics(hdc);

    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.FillPath(&brush, &path);
    graphics.DrawPath(&pen, &path);

    ReleaseDC(hParent, hdc);
}

void RegisterEditControl::getChildRect(HWND hChild, LPRECT rc)
{
    GetWindowRect(hChild, rc);
    SIZE si = {rc->right - rc->left, rc->bottom - rc->top};
    ScreenToClient(GetParent(hChild), (LPPOINT)rc);
    rc->right  = rc->left + si.cx;
    rc->bottom = rc->top + si.cy;
}

void RegisterEditControl::getRoundRectPath(GraphicsPath* pPath, const Gdiplus::Rect& r, int dia)
{
    // diameter can't exceed width or height
    if (dia > r.Width)
        dia = r.Width;
    if (dia > r.Height)
        dia = r.Height;

    // define a corner
    Rect Corner(r.X, r.Y, dia, dia);

    // begin path
    pPath->Reset();

    // top left
    pPath->AddArc(Corner, 180, 90);

    // top right
    Corner.X += (r.Width - dia - 1);
    pPath->AddArc(Corner, 270, 90);

    // bottom right
    Corner.Y += (r.Height - dia - 1);
    pPath->AddArc(Corner, 0, 90);

    // bottom left
    Corner.X -= (r.Width - dia - 1);
    pPath->AddArc(Corner, 90, 90);

    // end path
    pPath->CloseFigure();
}