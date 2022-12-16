#pragma once

#include "iwindow.h"
#include "snakegame.h"

#include <memory>

#ifdef _UNICODE
#    pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

class ImageContainer;

class MainWindow : public IWindow<MainWindow>
{
public:
    MainWindow();

    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    std::unique_ptr<SnakeGame> m_snakeGame;

    ImageContainer* pBitmap;
};
