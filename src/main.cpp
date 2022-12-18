#include "controls/registereditcontrol.h"
#include "snakeversion.h"
#include "widgets/mainwindow.h"

constexpr auto WindowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiPlusToken;
    Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, NULL);

    RegisterEditControl registerEditControl;

    RECT windowRect = { 0, 0, 1200, 600 };
    AdjustWindowRect(&windowRect, WindowStyle, FALSE);
    MainWindow win;
    if (!win.create((std::wstring(L"Змейка ") + SNAKEGAME_VERSION).c_str(), WindowStyle , 0,
                    100, 100, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                    nullptr, nullptr, nullptr, L"AppIcon")) {
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