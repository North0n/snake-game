#include "widgets/mainwindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiPlusToken;
    Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, NULL);

    MainWindow win;
    if (!win.create(L"SnakeGameMainWindow", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    nCmdShow = SW_SHOWMAXIMIZED;
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