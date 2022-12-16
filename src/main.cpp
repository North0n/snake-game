#include "widgets/mainwindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiPlusToken;
    Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, NULL);

    RECT windowRect = { 0, 0, 1200, 600 };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, FALSE);
    MainWindow win;
    if (!win.create(L"Змейка", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME , 0,
                    0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top)) {
        return 0;
    }

    ShowWindow(win.window(), nCmdShow);
    UpdateWindow(win.window());

    MSG msg = {};
    while (GetMessage(&msg, nullptr, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiPlusToken);
    return 0;
}